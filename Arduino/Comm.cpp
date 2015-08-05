#include "Comm.h"

#include <SoftwareSerial.h>

SoftwareSerial commSerial(10, 11);

void comm_init() {
  commSerial.begin(9600);
}

void command_send(byte cmd, byte len, void *data) {
  struct command send_data;
  send_data.checksum = 0;
  send_data.checksum += (send_data.id = 0xFF); // TODO: Read ID
  send_data.checksum += (send_data.cmd = cmd);
  send_data.checksum += (send_data.len = len);
  for (int i = 0; i < len; i++)
    send_data.checksum += (((char *)data)[i]);
  send_data.checksum = -send_data.checksum;

  commSerial.write((byte *)&send_data, sizeof(send_data));
  commSerial.write((byte *)data, len);
}

bool command_receive(struct command *recv_data, byte *buf_len, char *buffer) {
  commSerial.readBytes((char *)recv_data, 4);
  if (*buf_len < recv_data->len) {
    *buf_len = 0;
    char empty_buffer[recv_data->len];
    commSerial.readBytes(empty_buffer, recv_data->len);
    return false;
  }

  commSerial.readBytes(buffer, recv_data->len);
  *buf_len = recv_data->len;

  byte checksum = recv_data->id + recv_data->cmd + recv_data->len + recv_data->checksum;
  for (int i = 0; i < recv_data->len; i++)
    checksum += buffer[i];

  if (checksum != recv_data->checksum) {
    return false;
  }

  return true;
}

bool command_ready() {
  return Serial.available() > 3;
}

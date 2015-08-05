#pragma once
#include <Arduino.h>

struct command {
  byte id;
  byte cmd;
  byte len;
  byte checksum; // adding all bytes must be 0
};

struct command_get_card_info {
  char card[6];
};

struct command_card_info {
  byte is_valid;
};

void comm_init();
void command_send(byte cmd, byte len, void *data);
bool command_receive(struct command *recv_data, byte *buf_len, char *buffer);
bool command_ready();

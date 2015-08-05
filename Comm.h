#pragma once
#include <Arduino.h>

struct command {
  byte id;
  byte cmd;
  byte len;
  byte checksum; // adding all bytes must be 0
};

enum {
  CMD_CARD_INFO = 1,
  CMD_VERIFY_CARD = 2,
};

struct command_card_info_req {
  char card[6];
};

struct command_card_info_res {
  char challenge[16];
  char skip_pincode;
};

struct command_verify_card_req {
  char card[6];
  char challenge_response[32];
};

struct command_verify_card_res {
  char verification[32];
  char challenge_ok;
};

void comm_init();
void command_send(byte cmd, byte len, void *data);
bool command_receive(struct command *recv_data, byte *buf_len, char *buffer);
bool command_ready();

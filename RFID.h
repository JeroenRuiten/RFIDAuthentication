#pragma once
#include <Arduino.h>

void rfid_init();
void rfid();

void rfid_write_master(char key[6], short master_pin);

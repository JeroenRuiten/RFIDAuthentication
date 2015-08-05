#include <SoftwareSerial.h>
#include <EEPROM.h>

#include "Comm.h"
#include "RFID.h"

void setup() {
  comm_init();
  rfid_init();
}

void keypad() {

}


void loop() {
  // put your main code here, to run repeatedly:
  keypad();
  rfid();
}

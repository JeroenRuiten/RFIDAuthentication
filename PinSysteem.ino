#include <SoftwareSerial.h>
#include <EEPROM.h>

#include "Comm.h"
#include "RFID.h"

void setup() {
  comm_init();
  rfid_init();

  if (EEPROM[0] != 0x42) { // the meaning of life, the universe, and everything
    Serial.println("Waiting for master card");
    while (Serial.read() != 0x02);
    char card[6];
    rfid_parse_card(card);
    rfid_write_master(card, 0x1234);
    Serial.println("Registered card, pincode 1234");
  }
}

void keypad() {

}


void loop() {
  // put your main code here, to run repeatedly:
  keypad();
  rfid();
}

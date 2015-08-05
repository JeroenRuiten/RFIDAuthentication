#include "RFID.h"
#include <SoftwareSerial.h>
#include <EEPROM.h>

#define rfidSerial Serial

bool cardExists = false;
unsigned long lastTimeSeen;
char currentCard[6];

void rfid_init()
{
  rfidSerial.begin(9600);
}

// short ask_for_pin(); <--- TODO

void rfid_new_card(char card[6])
{ 

  char master_card[6];
  for (int i = 0; i < 6; i++)
    master_card[i] = EEPROM[i + 1];
  if (memcmp(card, master_card, 6) == 0) {
    Serial.println("Detected master card");
    // parse master card
  } else {
    Serial.println("New card!");
  }
  
  cardExists = true;
  memcpy(currentCard, card, 6);
  lastTimeSeen = millis();
}

void rfid_drop_card()
{
  cardExists = false;
  Serial.println("Dropped card!");
}

void rfid_ping_card()
{
  lastTimeSeen = millis();
  Serial.println("Ping card");
}

void rfid()
{
  if (!rfidSerial.available()) {
    if (cardExists && millis() - lastTimeSeen > 1000) {
      rfid_drop_card();
    }
    return;
  }

  while (rfidSerial.read() != 0x02) // STX
    return;

  char result[6];
  rfid_parse_card(result);

  if (!cardExists) {
    rfid_new_card(result);
  } else {
    if (memcmp(result, currentCard, 6) == 0)
      rfid_ping_card();
    else {
      rfid_drop_card();
      rfid_new_card(result);
    }
  }
}

void rfid_parse_card(char buffer[6])
{
  char hex_buffer[13];
  rfidSerial.readBytes(hex_buffer, 12);
  hex_buffer[12] = 0;
  memset(buffer, 0, 6);
  for (int i = 0; i < 12; i++) {
    int res;
    if (hex_buffer[i] >= 'A' && hex_buffer[i] <= 'F')
      res = hex_buffer[i] - 'A' + 0xA;
    else
      res = hex_buffer[i] - '0';
    buffer[i / 2] = buffer[i / 2] << 4 | res;
  }
  char end_bytes[3];

  char checksum = 0;
  for (int i = 0; i < 5; i++) {
    checksum ^= buffer[i];
  }

  if (buffer[5] != checksum) {
    Serial.println("Invalid checksum");
    return;
  }
}

void rfid_write_master(char key[6], short master_pin)
{
  EEPROM.update(0, 0x42);
  for (int i = 0; i < 6; i++)
    EEPROM.update(i + 1, key[i]);
  EEPROM.update(7, master_pin & 0xFF);
  EEPROM.update(8, master_pin >> 8);
}


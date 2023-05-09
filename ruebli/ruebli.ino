#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include "WiFiHandler.h"

void setup() {
  Serial.begin(9600);
  Serial.println();
  EEPROM.begin(128);
  delay(2000);
  WiFiHandler wifi;
}

void loop() {}

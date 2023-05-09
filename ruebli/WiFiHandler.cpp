#include "Arduino.h"
#include "WiFiHandler.h"

#include <EEPROM.h>
#include <ESP8266WiFi.h>

WiFiHandler::WiFiHandler() {
  String ssid = "";
  bool ssidFlag = false;
  String password = "";
  bool saveNetworkFlag = false;

  for (int address = 0; address < 128; address++) {
    byte val = EEPROM.read(address);
    if (val == 255) {
      break;
    } else if (val == 0) {
      ssidFlag = true;
    } else {
      if (ssidFlag) {
        password += (char)val;
      } else {
        ssid += (char)val;
      }
    }
  }
  
  if (ssid == "") {
    Serial.println("No network info stored in memory.");
    saveNetworkFlag = true;
    int nOfNetworks = WiFi.scanNetworks(false, true);
    Serial.println("Discovered networks:");
    for (int i = 0; i < nOfNetworks; i++) {
      Serial.printf("%d: %s\n", i + 1, WiFi.SSID(i));
    }
    Serial.println("Enter the index for the chosen network:");
    int chosen;
    while (true) {
      if (Serial.available() > 0) {
        chosen = Serial.parseInt() - 1;
        Serial.read();
        break;
      } else {
        delay(500);
      }
    }
    ssid = WiFi.SSID(chosen);
    Serial.println(WiFi.encryptionType(chosen));
    if (WiFi.encryptionType(chosen) != 7) {
      Serial.println("Enter the password for the chosen network:");
      while (true) {
        if (Serial.available() > 0) {
          password = Serial.readString();
          password.trim();
          break;
        } else {
          delay(500);
        }
      }
    }
  } else {
    Serial.printf("Previous network info for %s found in EEPROM\n", ssid);
  }
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  if (saveNetworkFlag) {
    int address = 0;
    for (int i = 0; i < ssid.length(); i++) {
      EEPROM.write(address, ssid.charAt(i));
      address++;
    }
    EEPROM.write(address, '\00');
    address++;
    
    for (int i = 0; i < password.length(); i++) {
      EEPROM.write(address, password.charAt(i));
      address++;
    }
    EEPROM.end();
  }
}

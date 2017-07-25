#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "RemoteDebug.h"

// Network credentials
const char* ssid = "vestergaard";
const char* password = "dannevirke";

uint32_t mLastTime = 0;
uint32_t mTimeSeconds = 0;
int number = 2;
char str[] = "Hello World!";
int indexData = 0;
char data[10];

// instantiate RemoteDebug object
RemoteDebug Debug;

void setup() {
  // use Serial (port 0); initialize Modbus communication baud rate
  Serial.begin(38400);
 // Serial.setTimeout(100);

  // communicate with Modbus slave ID 1 over Serial (port 0)
 // node.begin(1, Serial);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }

  // Initialize the telnet server of RemoteDebug
  Debug.begin("Telnet_HostName"); // Initiaze the telnet server

  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();
  
  static uint32_t i;
  String incomingString;
  
  byte message[] = {0x43, 0x01, 0x00, 0x03, 0x02 };
  
  if ((millis() - mLastTime) >= 2000) {

    // Time

    mLastTime = millis();

    mTimeSeconds++;
    i++;
    
  
    Serial.write(message, sizeof(message));

    while (Serial.available() > 0) {
      // read the incoming byte:
      data[indexData]= Serial.read();
      indexData++;
//      incomingString = Serial.readString();
    }

    if (indexData > 0){
      Debug.printf("Number of bytes received: %d\n", indexData);
      Debug.printf("%s %s %s %s %s %s %s %s %s %s \n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9]);
      Debug.println("\n");
      indexData = 0; 
    }
  }
    

  Debug.handle();
  yield();
  
}

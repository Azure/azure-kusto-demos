// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// This example only works with Arduino IDE 1.6.8 or later.

#include <ESP8266WiFi.h>
#include <time.h>
#include "command_center.h"
#include <AzureIoTHub.h>
#ifdef AzureIoTUtilityVersion
#include <AzureIoTProtocol_MQTT.h>
#endif

const char ssid[] = ""; //  your WiFi SSID (name)
const char pass[] = "";    // your WiFi password (use for WPA, or use as key for WEP)
const char connectionString[] = ""; // iot hub connection string
int status = WL_IDLE_STATUS;

#define LED_PIN 13

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN,LOW);
    initSerial();
    initWifi();
    initTime();
    // This function doesn't exit.
   command_center_run();
}

void loop() {
  // Not used.
}

void initSerial() {
    // Start serial and initialize stdout
    Serial.begin(9600);
   // Serial.setDebugOutput(true);
}

void initWifi() {
    // Attempt to connect to Wifi network:
    //Serial.print("Attempting to connect to SSID: ");
 //   Serial.println(ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
   status = WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        digitalWrite(LED_PIN,HIGH);
        delay(500);
        digitalWrite(LED_PIN,LOW);
         delay(500);
 //       Serial.print(".");
    }
  digitalWrite(LED_PIN,LOW);
  //  Serial.println("Connected to wifi");
}

void initTime() {
    time_t epochTime;

    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    while (true) {
        epochTime = time(NULL);

        if (epochTime == 0) {
   //         Serial.println("Fetching NTP epoch time failed! Waiting 2 seconds to retry.");
            delay(2000);
        } else {
   //         Serial.print("Fetched NTP epoch time is: ");
   //         Serial.println(epochTime);
            break;
        }
    }
}

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Update.h>
#include "ota/ota.h"

#define WIFI_APN "wifiap"
#define WIFI_PASS "wifipass"

void setup() {
	Serial.begin(115200);
	Serial.println ("Hello world!");
	
	WiFi.begin(WIFI_APN, WIFI_PASS);

	// TODO edit url in ota.h !!
	OTA::ota_setup();
	// TODO set to false
	OTA::stop = true;
}

void loop() {
	Serial.println ("Loop!!");
	delay(5000);
}
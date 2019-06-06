#include "ota.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Update.h>
#include <buildinfo.h>

namespace OTA {

	// DlInfo info;
	char chipid[13] = {0};
	char addr[100];
	char ver[100];
	char _hash[100];
	char *url;
	char *md5;

	bool stop = false;
	bool is_updating = false;

	bool checkUpdateFile();
	void ota_task(void * p);
	int update();

	void ota_setup() {

		uint8_t mac[6];
		esp_efuse_mac_get_default(mac);
		sprintf(chipid, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		#if(HIDECHIPID)
		Serial.printf("Chip id: %s\n", chipid);
		#endif // HIDECHIPID

		delay(10000);
		xTaskCreate(ota_task, "OtaTask", 10000, NULL, 1, NULL); 

	}


	bool checkUpdateFile() {

		HTTPClient http;
		sprintf(addr, OTAURL ".txt", chipid);
		http.begin(addr);

		int httpCode = http.GET();

		Serial.printf("GET %s %d\n", addr, httpCode);

		if(httpCode > 0 && httpCode == HTTP_CODE_OK) {
			int len = http.getSize();

			auto inf = http.getString().c_str();

			if (strlen(inf) > (14+32+1)) {
				Serial.printf("[ERR] info too long %d\n", len);
				http.end();
				return false;
			}

			// Serial.println(inf);
			sscanf(inf, "%s %s", ver, _hash);
			// Serial.printf("ver=%d, hash=%s\n", ver, hash);
			Serial.printf("Current version %s\n", _BuildInfo.src_version);

			if (strcmp(ver, _BuildInfo.src_version) != 0) {
				sprintf(addr, OTAURL ".bin", chipid);
				url = addr;
				md5 = _hash;
				// Serial.printf("Start update from %s version=%d hash=%s\n", addr, ver, hash);
				Serial.printf("New version found %s\n", ver);
				http.end();
				return true;
			}
			else {
				Serial.printf("Skip this version %s\n", ver);
			}
		}

		http.end();
		return false;
	}

	void ota_task(void * p) {
		wl_status_t oldst = WL_DISCONNECTED;
		while(1) {
			delay(1000);
			if(stop) continue;
			wl_status_t st = WiFi.status();
			if (st != oldst) {
				if (st == WL_CONNECTED) {
					Serial.printf("WiFi connected, IP address: %s\n", WiFi.localIP().toString().c_str()); 
				}
				else if (st == WL_DISCONNECTED) {
					Serial.printf("WiFi disconnected\n"); 
				}
			}
			if (st != WL_CONNECTED) {
				Serial.println("Wifi still wait...");
				WiFi.reconnect();
				delay(CHECKUPDATEINTERVAL * 1000);
				continue;
			}
			oldst = st;

			if (checkUpdateFile() && !stop) {
				Serial.printf("Start update from %s hash=%s\n", url, md5);
				is_updating = true;
				update();
				is_updating = false;
				delay(CHECKUPDATEINTERVAL * 1000);
			}
			else {
				delay(CHECKUPDATEINTERVAL * 1000);
			}
		}
	}

	// from https://github.com/nhatuan84/esp32-http-ota
	int update() {
		int downloaded = 0;
		int flashed = 0;
		int written = 0;
		int total = 1;
		uint8_t buff[1024] = { 0 };
		size_t size = sizeof(buff);
		int ret = 0;
		HTTPClient http;

		http.begin(url);

		int httpCode = http.GET();

		if(httpCode > 0 && httpCode == HTTP_CODE_OK) {
			// get lenght of document (is -1 when Server sends no Content-Length header)
			int len = http.getSize();
			if (!Update.begin(len, U_FLASH)) {
				Serial.println("Flashing init ... failed!");
				ret = -1;
				return ret;
			}
			Update.setMD5(md5);
			total = len;
			downloaded = 0;
			// get tcp stream
			WiFiClient * stream = http.getStreamPtr();
			// info.startDownloadCallback();
			// read all data from server
			while(http.connected() && (len > 0)) {
				// get available data size
				size = stream->available();

				if(size > 0) {
					// read up to 128 byte
					int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
					
					if ((downloaded+c) > 1024*1024) {
						Serial.println("Firmware is too big!");
						ret = -1;
						break;
					}

					written = Update.write(buff, c);
					if (written > 0) {
						if(written != c){
							Serial.println("Flashing chunk not full ... warning!");
						}
						flashed += written;
						// info.progressCallback(Flashing_e, (100*downloaded)/total);
					} else {
						Serial.println("Flashing ... failed!");
						ret = -1;
						break;
					}
					
					// write to storage
					// info.saveDataCallback(buff, c);
					downloaded += c;
					// info.progressCallback(DownloadingAndSaving_e, (100*downloaded)/total);

					if(len > 0) {
						len -= c;
					}
				}
				delay(1);
			}
			// info.endDownloadCallback();
		}else {
			Serial.println("[HTTP] GET... failed!");
			ret -1;
		}

		http.end();

		if (ret == 0 && downloaded > 0) {
			if (Update.end()) {
				Serial.println("Flashing ... done!");
				delay(100);
				ESP.restart();
			} else {
				Serial.println("Flashing or md5 ... failed!"); 
				ret = -1;
			}
		}

		return ret;
	}
};
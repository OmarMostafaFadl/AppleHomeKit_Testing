/*
 * switch.ino
 *
 *  Created on: 18/8/2021
 *      Editors: Omar Fadl, Ahmed Hesham, Ahmed Samy
 *
 */

#ifndef WIFI_INFO_H_
#define WIFI_INFO_H_

#if defined(ESP8266)
#include <ESP8266WiFi.h>       //Includes the WiFi Library. You can find it's documentaion at: https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html
#elif defined(ESP32)
#include <WiFi.h>
#endif

const char *ssid = "OMARali3";
const char *password = "1234567892";

void wifi_connect() {
	WiFi.persistent(false);                  //This being false makes WiFi.begin, WiFi.disconnect, WiFi.softAP, or WiFi.softAPdisconnect only changes the current in-memory
	                                         //WiFi settings, and does not affect the WiFi settings stored in flash memory.
	WiFi.mode(WIFI_STA);                     //Sets the ESP8266 as a Station. Could also be WIFI_AP > Access Point. Or WIFI_STA_AP > Access Point and Station
	WiFi.setAutoReconnect(true);             //Configure module to automatically connect on power on to the last used access point
	WiFi.begin(ssid, password);
	Serial.println("WiFi connecting...");
	while (!WiFi.isConnected()) {            //Checks if the ESP8266 is connected, Sometimes it takes several seconds.
		delay(100);
		Serial.print(".");
	}
	Serial.print("\n");
	Serial.printf("WiFi connected, IP: %s\n", WiFi.localIP().toString().c_str());
}

#endif /* WIFI_INFO_H_ */

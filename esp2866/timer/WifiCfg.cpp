#include "Arduino.h"
#include <ESP8266WiFi.h>
#include "WifiCfg.h"

WifiCfg::WifiCfg(){
	Serial.println('in WifiCfg constructor');
}

void WifiCfg::getSSIDs(){
	Serial.println('in getSSIDs');
}
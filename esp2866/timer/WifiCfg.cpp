#include "Arduino.h"
#include <ESP8266WiFi.h>
#include "WifiCfg.h"


void WifiCfg::getSSIDs(){
  char str[50];
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);  
  int n = WiFi.scanNetworks();
  Serial.print(n);
  Serial.println(" networks found");  
}
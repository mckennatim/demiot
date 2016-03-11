#include "Arduino.h"
#include <ESP8266WiFi.h>
#include "WebCfg.h"
#include <EEPROM.h>

WebCfg::WebCfg(int port): server(80)
{
	_port=port;
	EEPROM.begin(512);
	//getSSIDs();
}

void WebCfg::getSSIDs()
{
  char str[50];
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(200);  
  int n = WiFi.scanNetworks();
  Serial.print(n);
  Serial.println(" networks found"); 
	strcpy(ssids,"{");
	char tr[n][25];
	int tr2[n][2];
	for (int i = 0; i < n; ++i){
		strcpy(tr[i],WiFi.SSID(i).c_str());
		tr2[i][0]=WiFi.RSSI(i);
		tr2[i][1]=WiFi.encryptionType(i);
	}
	for (int i = 0; i < n; ++i){
		sprintf(str,"[\"ssid\": \"%s\", \"rssi\": %d, \"enc\": %d ]", tr[i], tr2[i][0], tr2[i][1]);
		strcat(ssids,str);
		if(i<n-1){strcat(ssids, ",");}
	}
	strcat(ssids,"}");
}

void WebCfg::startAp(){
	WiFi.softAP("ulysses", "mabibi", 6);	
	server.begin();
}

void WebCfg::setupServer()
{
  server.on("/", [&]() {
    IPAddress ip = WiFi.softAPIP();
    server.send(200, "text/html", ssids);  
  });	
  server.on("/setting", [&]() {
    strcpy(ssid, server.arg("ssid").c_str());
    strcpy(pwd, server.arg("pwd").c_str());
    strcpy(devid, server.arg("devid").c_str());
    strcpy(ip, server.arg("ip").c_str());
    strcpy(port, server.arg("port").c_str());
    server.send(200, "application/json", "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}");
    this->showCfg();
    this->saveCfg(ssid);
  });
}

// void WebCfg::serverBegin()
// {
// 	server.begin();
// }

void WebCfg::saveCfg(char cf[])
{
  for (int i = 0; i < strlen(cf); ++i)
  {
    EEPROM.write(i, cf[i]);
    Serial.print("Wrote: ");
    Serial.println(cf[i]); 
  }	
}

void WebCfg::showCfg()
{
	Serial.println(ssid);
	Serial.println(pwd);
	Serial.println(devid);
	Serial.println(ip);
	Serial.println(port);
}
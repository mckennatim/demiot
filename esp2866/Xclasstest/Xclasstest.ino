#include "Aclass.h"
#include "WebCfg.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

Aclass aclass;
//Bclass bclass;
//WebCfg cfg(80);
ESP8266WebServer server(80);
WebCfg cfg(server);
const char *assid = "ESPap";

char ssids[300];
void getSSIDs()
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
// 		char ssid[40];
// 		char pwd[24];
// 		char devid[9];
// 		char ip[16];
// 		char port[5];

// void setupServer()
// {
//   server.on("/", [&]() {
//     //IPAddress ip = WiFi.softAPIP();
//     server.send(200, "text/html", "ssids");  
//   });	
//   server.on("/setting", [&]() {
//     strcpy(ssid, server.arg("ssid").c_str());
//     strcpy(pwd, server.arg("pwd").c_str());
//     strcpy(devid, server.arg("devid").c_str());
//     strcpy(ip, server.arg("ip").c_str());
//     strcpy(port, server.arg("port").c_str());
//     server.send(200, "application/json", "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}");
//     // this->showCfg();
//     // this->saveCfg(ssid);
//   });
// }

void handleRoot() {
	server.send(200, "text/html", "<h1>You are connected</h1>");
}

void setup(){
	Serial.begin(115200);
	Serial.println();
	Serial.println("--------------------------");
  Serial.println("ESP8266 Class Test");
  Serial.println("--------------------------");
  aclass.dot();
  //bclass.dash();
  //cfg.getSSIDs();
  //getSSIDs();
  //Serial.println(ssids);
  //WiFi.mode(WIFI_AP);
  WiFi.softAP(assid);
	server.on("/", handleRoot);
	server.begin();
	Serial.println("HTTP server started");
  //server.begin();
  //cfg.setupServer();
  //setupServer();
  //cfg.startAp();
}

void loop(){
	//cfg.server.handleClient();
	server.handleClient();
}
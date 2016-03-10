#include "Arduino.h"
#include <ESP8266WiFi.h>
#include "WebCfg.h"


WebCfg::WebCfg(int port): server(80)
{
	_port=port;
}

void WebCfg::getSSIDs(){
  char str[50];
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);  
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

void WebCfg::setupServer(){
  server.on("/", [&]() {
    IPAddress ip = WiFi.softAPIP();
    server.send(200, "text/html", ssids);  
  });	
}
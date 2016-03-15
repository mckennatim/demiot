#include "Config.h"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
Config cfg(server);

void setup(){
	Serial.begin(115200);
	Serial.println();
	Serial.println("--------------------------");
  Serial.println("ESP8266 multifile");
  Serial.println("--------------------------");
  cfg.getOnline();
}

void loop(){
	if(IN_CONFIG_MODE){
	 server.handleClient();
	}
}
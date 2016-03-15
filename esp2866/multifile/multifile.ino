#include "config.h"

void setup(){
	Serial.begin(115200);
	Serial.println();
	Serial.println("--------------------------");
  Serial.println("ESP8266 multifile");
  Serial.println("--------------------------");
  getOnline();
}

void loop(){
	
	server.handleClient();
	
}
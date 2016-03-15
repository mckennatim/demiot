#include "config.h"
#include <EEPROM.h>

void setup(){
	Serial.begin(115200);
	EEPROM.begin(512);
	Serial.println();
	Serial.println("--------------------------");
  Serial.println("ESP8266 multifile");
  Serial.println("--------------------------");
  getOnline();
}

void loop(){
	
	server.handleClient();
	
}
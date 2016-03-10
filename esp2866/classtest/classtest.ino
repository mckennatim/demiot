#include "Aclass.h"
#include "WebCfg.h"

Aclass aclass;
WebCfg cfg(80);

void setup(){
	Serial.begin(115200);
	Serial.println();
	Serial.println("--------------------------");
  Serial.println("ESP8266 Class Test");
  Serial.println("--------------------------");
  aclass.dot();
  cfg.getSSIDs();
  Serial.println(cfg.ssids);
}

void loop(){

}
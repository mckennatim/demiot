#include "Aclass.h"

Aclass aclass;


void setup(){
	Serial.begin(115200);
	Serial.println();
	Serial.println("--------------------------");
  Serial.println("ESP8266 Class Test");
  Serial.println("--------------------------");
  aclass.dot();
}

void loop(){

}
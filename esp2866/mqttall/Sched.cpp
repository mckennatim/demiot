#include "Sched.h" //private: 
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include "config.h"
#include "MQclient.h" //for extern NEW_MAIL
#include "STATE.h"

bool Sched::deserialize(char* kstr){
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(kstr);
  root.prettyPrintTo(Serial);
  unix = root["unix"];
  LLLL = root["LLLL"];
  zone = root["zone"];
  return root.success();
}

void Sched::act(STATE& st){
	Serial.println(unix);
	Serial.println(LLLL);
	Serial.println(zone);
  time_t datetime = unix + zone*60*60;
  Serial.println(datetime); 
  setTime(datetime);
  setSyncInterval(4000000); 
  Serial.println(hour());  	
	NEW_MAIL=0;
}

int t0[4][3];
int t1[4][3];

bool Sched::deseriProgs(char* kstr){
	StaticJsonBuffer<600> jsonBuffer2;
	JsonArray& root = jsonBuffer2.parseArray(kstr);
	JsonArray& temp0 = root[0]["temp0"];
	JsonArray& temp1 = root[1]["temp1"];
	for(int i = 0; i<4; i++){
		for (int j = 0; j<3; j++){
			t0[i][j]=temp0[i][j];
		}
	}
	for(int i = 0; i<4; i++){
		for (int j = 0; j<3; j++){
			t1[i][j]=temp1[i][j];
		}
	}
  return root.success();
}
// bool Sched::deseriProgs(char* kstr){
// 	Serial.println("in deseriprogs");
// 	char k[] = "[{\"temp0\":[[6,12,68],[8,20,57]]},{\"temp1\":[[6,0,67],[18,0,68],[21,30,58]]}]";
// 	StaticJsonBuffer<600> jsonBuffer2;
// 	//DynamicJsonBuffer jsonBuffer2;
// 	JsonArray& root = jsonBuffer2.parseArray(kstr);
// 	root.prettyPrintTo(Serial);
// 	JsonArray& temp0 = root[0]["temp0"];
// 	temp0.prettyPrintTo(Serial);
// 	JsonArray& temp1 = root[1]["temp1"];
// 	temp0[1][2].prettyPrintTo(Serial);
// 	Serial.print("the # of 3tuples is: ");
// 	Serial.println(temp0[0].size());
// 	Serial.println("temp0");	
// 	for(int i = 0; i<4; i++){
// 		Serial.print("[");
// 		for (int j = 0; j<3; j++){
// 			t0[i][j]=temp0[i][j];
// 			Serial.print(t0[i][j]);
// 			Serial.print(",");
// 		}
// 		Serial.print("]");
// 		Serial.println("");
// 	}
// 	Serial.println("temp1");	
// 	for(int i = 0; i<4; i++){
// 		Serial.print("[");
// 		for (int j = 0; j<3; j++){
// 			t1[i][j]=temp1[i][j];
// 			Serial.print(t1[i][j]);
// 			Serial.print(",");
// 		}
// 		Serial.print("]");
// 		Serial.println("");
// 	}
//   return 1;
// }

void Sched::actProgs(STATE& st){
	int aday[4][3]= {{6,12,68},
	          {8,20,57},
	          {10,0,68},
	          {11,30,58}
	          };
	int bday[][3]= {6,12,68,8,20,57,10,0,68,11,30,58};	
	Serial.println("in act progs");
	Serial.println(aday[1][2]);
	Serial.println(bday[1][2]);
	NEW_MAIL=0;
}
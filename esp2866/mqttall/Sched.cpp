#include "Sched.h" //private: 
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include "config.h"
#include "MQclient.h" //for extern NEW_MAIL
#include "STATE.h"

char * schedArr[]={"temp1","temp2","tmr1","tmr2","tmr3"};
//int NEW_ALARM = -1;

bool Sched::deserialize(char* kstr){
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(kstr);
  root.prettyPrintTo(Serial);
  unix = root["unix"];
  LLLL = root["LLLL"];
  zone = root["zone"];
  return root.success();
}

void Sched::actTime(STATE& st){
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

int Sched::idxOsenrels(int j){
	for(int i=0;i<seresz;i++){
		if(senrels[i]==j){
			return i;
		} 
	} 
}

bool Sched::deseriProgs(char* kstr){
	//DynamicJsonBuffer jsonBuffer;
  StaticJsonBuffer<1000> jsonBuffer;
	JsonObject& rot = jsonBuffer.parseObject(kstr);
	JsonArray& sere = rot["serels"];
	JsonArray& root = rot["progs"];
	seresz = sere.size();
	for(int h=0;h<seresz;h++){
		senrels[h] = sere[h];
	}
	nsr = root.size();
	Serial.println("");  
	Serial.print("the # of sinks is: ");
	Serial.println(nsr);	
	for(int i = 0; i<nsr; i++){
		JsonArray& asnk = root[i]; //asnk[4][3]
		events[i] = root[i].size(); //4
		Serial.println(schedArr[idxOsenrels(i)]);
		// Serial.println(idxOsenrels(i));
		for(int j = 0; j<events[i]; j++){//4
			int bsz = asnk[j].size();
			haynRset[j] = bsz-2;
			for(int k=0; k<bsz;k++){
				progs[i][j][k] = asnk[j][k];
				Serial.print(progs[i][j][k]);
				Serial.print(",");
			}
			Serial.println("");  
		}
		Serial.println("");  
	}
	Serial.println(progs[0][1][2]);
	Serial.println(progs[1][1][2]);
  return rot.success();
}

void Sched::resetAlarm(int i, int &cur){
	//int cur =0;
	Serial.println("in resetAlarm");
	int idx = senrels[i];
	char senrel[10];
	strcpy(senrel,schedArr[i]);
	for(int j=0; j<events[idx];j++){
		if (hour() == progs[idx][j][0]){
			if (progs[idx][j][1] > minute()){
				cur = j;
				break;
			}
		}
		if (progs[idx][j][0] > hour()){
			cur= j;
			break;
		}
	}
	Serial.println(cur);
	Serial.println(senrel);
	Serial.print(hour());
	Serial.print(":");
	Serial.print(minute());
	Serial.println();
	Serial.print("[");
	Serial.print(progs[idx][cur][0]);
	Serial.print(":");
	Serial.print(progs[idx][cur][1]);
	Serial.print("->");
	Serial.print(progs[idx][cur][2]);
	Serial.println("]");
	//actProgs(i, cur);
}

void Sched::actProgs(int idx, int cur, STATE& st){
	int ii = senrels[idx];
	switch(idx){
		case 0:
			Serial.println("case temp1");
			// Serial.println(cur);
			// Serial.println(85);
			// Serial.println(progs[0][2][2]);
			// Serial.println(progs[ii][cur-1][2]);
			// Serial.println(progs[ii][cur-1][3]);
			st.hilimit = progs[ii][cur-1][2];
			st.lolimit = progs[ii][cur-1][3];
			st.HAY_CNG=1;
			Alarm.alarmOnce(progs[ii][cur][0],progs[ii][cur][0], 0, cbtemp1);
			break;
		case 1:
			Serial.println("case temp2");
			break;
		case 2:
			Serial.println("case tmr1 is up");
			Alarm.alarmOnce(progs[ii][cur][0],progs[ii][cur][0], 0, cbtmr1);
			break;
		case 3:
			Serial.println("case tmr2");
			break;
		case 4:
			Serial.println("case tmr3");
			break;
	}
	NEW_MAIL=0;
	NEW_ALARM=-1;
}

void cbtmr1(){
	NEW_ALARM=2;
}
void cbtemp1(){
	NEW_ALARM=0;
}



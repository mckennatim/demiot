#include "Sched.h" //private: 
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <PubSubClient.h>
#include "config.h"
#include "MQclient.h" //for extern NEW_MAIL
#include "STATE.h"
#include "TMR.h"

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
	//Alarm.alarmOnce(hour(), minute()+1,0,abdd);
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

//var sched3 = "{\"serels\":[0,99,1,2],\"progs\":[[[0,0,80,77],[6,12,82,75],[8,20,85,75],[22,0,78,74],[23,30,85,75]],[[0,0,58],[18,0,68],[21,30,58]],[[5,30,1],[6,10,0]]]}";
//char * schedArr[]={"temp1","temp2","tmr1","tmr2","tmr3"};
//events =  [5,3,2]
//haynRset= [2,1,1]

void Sched::resetAlarm(int i, int &cur){
	//int cur =0;
	Serial.println("in resetAlarm");
	int idx = senrels[i];
	char senrel[10];
	strcpy(senrel,schedArr[i]);
	//make cur the 
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

void Sched::actProgs(int idx, int cur, STATE& st, TMR& tmr){
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
			Alarm.alarmOnce(progs[ii][cur][0],progs[ii][cur][1], 0, cbtemp1);
			break;
		case 1:
			Serial.println("case temp2");
			break;
		case 2:
			Serial.print("setpoint tmr1 is set for: ");
			Serial.print(progs[ii][cur][0]);
			Serial.print(":");
			Serial.println(progs[ii][cur][1]);			
			Alarm.alarmOnce(progs[ii][cur][0],progs[ii][cur][1], 0, cbtmr1);
			break;
		case 3:
			Serial.print("countdown tmr2 starts at: ");
			if(progs[ii][cur][2]==1){
				int fhr = progs[ii][cur+1][0];
				int fmi = progs[ii][cur+1][1];
				int shr = progs[ii][cur][0];
				int smin = progs[ii][cur][1];
				int mi;
				int hr;
				if(fmi< smin){
					mi = 60-smin+fmi;
					shr++;
				}else {
					mi = fmi -smin;
				}
				hr = fhr - shr;
				int dur = 60*hr + mi;
				tmr.timr2= dur;
				Serial.println(dur);
			}else {
				Serial.println("countdown is OVER");
			}
			Serial.println("cbtmr2ING cbtmr2ING cbtmr2ING");
			Serial.print("countdown timr2 is set for: ");
			Serial.print(progs[ii][cur+1][0]);
			Serial.print(":");
			Serial.println(progs[ii][cur+1][1]);
      //Alarm.alarmOnce(hour(), minute()+1,0,cbtmr2);			

			//Alarm.alarmOnce(progs[ii][cur+1][0],progs[ii][cur+1][1], 0, cbtmr2);			
			break;
		case 4:
			Serial.println("case tmr3");
      Serial.println("TING TING TING");
			Serial.print(hour());
			Serial.print(":");
			Serial.println(minute()+1);      
      Alarm.alarmOnce(hour(), minute()+1,0,abdd);			
			break;
	}
	NEW_MAIL=0;
	NEW_ALARM=-1;
}

void Sched::actProgs2(TMR& tmr){
	Serial.print("in actProgs2, NEW_ALARM=");
	Serial.println(NEW_ALARM);
	if((NEW_ALARM & 8) == 8){
		Serial.print("reset for 1 min and shutting off 8 w mask23 :");
		NEW_ALARM = NEW_ALARM & 23;
		Serial.println(NEW_ALARM);
		Serial.print(hour());
		Serial.print(":");
		Serial.println(minute()+1); 	
		Alarm.alarmOnce(hour(), minute()+1,0,bm8);
	}
	if((NEW_ALARM & 4) == 4){
		Serial.print("reset for 1 min and shutting off 4 w mask27 :");
		NEW_ALARM = NEW_ALARM & 27;
		Serial.println(NEW_ALARM);
		Serial.print(hour());
		Serial.print(":");
		Serial.println(minute()+1); 	
		Alarm.alarmOnce(hour(), minute()+1,6,bm4);
	}
}

void Sched::updateTmrs(TMR& tmr, PubSubClient& client){
	int bitmap = 1;

		tmr.timr2 = tmr.timr2 - tmr.crement;
		if(tmr.timr2 <= 0){
			tmr.timr2 = 0;
		}
	
	Serial.println("15 seconds pass");
}

void cbtmr1(){
	Serial.println("in cbtmr1");
	NEW_ALARM=2;
}
void cbtmr2(){
	Serial.println("in cbtmr2");
	NEW_ALARM=3;
}
void cbtemp1(){
	Serial.println("in cbtemp1");
	NEW_ALARM=0;
}

void abdd(){
	Serial.println("in abdd");
  int i=0;
  switch(i){
    case 0:
      // Serial.println("TING TING TING");
      // Alarm.alarmOnce(hour(), minute()+1,0,abdd);
      NEW_ALARM=4;
  }	
}

void bm8(){
	Serial.print("mask with 8 begets: ");
	NEW_ALARM = NEW_ALARM | 8;
	Serial.println(NEW_ALARM);
}
void bm4(){
	Serial.print("mask with 4 begets: ");
	NEW_ALARM = NEW_ALARM | 4;
	Serial.println(NEW_ALARM);
}
void bm2(){
	Serial.print("mask with 2 begets: ");
	NEW_ALARM = NEW_ALARM | 2;
	Serial.println(NEW_ALARM);
}
void bm1(){
	Serial.print("mask with 1 begets: ");
	NEW_ALARM = NEW_ALARM | 1;
	Serial.println(NEW_ALARM);
}


#include "config.h"
#include <EEPROM.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "MQclient.h" //globals(extern) NEW_MAIL, itopic, ipayload
#include "STATE.h"
#include "Cmd.h"

#define ONE_WIRE_BUS 4  // DS18B20 pin

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

WiFiClient espClient;
PubSubClient client(espClient);
Console console(devid, client);
MQclient mq(devid);

STATE st {5, 42, 38, 0, 82, 73, 1, 1, 0};
//{ALED, temp1, temp2, heat, hilimit, lolimit, AUTOMA, HAY_CNG, NEEDS_RESET}

const int numcmds = 3;
char incmd[][10]={"clock", "progs", "cmd"};


void processInc(){
  for (int i=0;i<numcmds;i++){
    if(strcmp(incmd[i], itopic)==0){
      switch (i){
        case 0:
          Serial.println("in clock");
          break;            
        case 1:
          Serial.println("in progs");
          break;            
        case 2:
          Serial.println("in cmd");
          Cmd cmd;
          cmd.deserialize(ipayload);
          cmd.act(st);
          // Cmdd cmd;
          // cmdDeserialize(cmd, ipayload);
          // cmdAct(cmd);
          break; 
        default:           
          Serial.println("in default");
          break; 
      }
    }
  }
}

void publishState(){
	char astr[120];
	sprintf(astr, "{\"temp1\":%d, \"temp2\":%d, \"heat\":%d, \"hilimit\":%d, \"lolimit\":%d, \"auto\":%d  }", st.temp1, st.temp2, st.heat, st.hilimit, st.lolimit, st.AUTOMA);
	char status[20];
	strcpy(status,devid);
	strcat(status,"/status");
  if (client.connected()){
    client.publish(status, astr, true);
  }	
  Serial.print(status);
	Serial.println(astr);
}

void readTemps(){
	DS18B20.requestTemperatures(); 
	int temp1 = (int)DS18B20.getTempFByIndex(0);
	int temp2 = (int)DS18B20.getTempFByIndex(1);
	if(temp1 != st.temp1){
		st.temp1=temp1;
		st.HAY_CNG=1;
	}
	if(temp2 != st.temp2){
		st.temp2=temp2;
		st.HAY_CNG=1;
	}
}

void controlHeat(){
	bool heat = st.heat;
	if (st.temp1 >= st.hilimit){
		heat=0;
	}	
	if (st.temp1 <= st.lolimit){
		heat=1;
	}	
	if (heat != st.heat){
		st.heat = heat;
		digitalWrite(st.ALED, st.heat);
		st.HAY_CNG= 1;
	}
}

void setup(){
	Serial.begin(115200);
	EEPROM.begin(512);
	Serial.println();
	Serial.println("--------------------------");
  Serial.println("ESP8266 mqttdemo");
  Serial.println("--------------------------");
  getOnline();
  client.setServer(ip, 1883);
  client.setCallback(handleCallback);  
  pinMode(st.ALED, OUTPUT);
  digitalWrite(st.ALED, st.heat);
}

long before = 0;
long now;

void loop(){
	server.handleClient();
	if(NEW_MAIL){
    processInc();
  }
	if(!client.connected() && !NEEDS_RESET){
		 mq.reconn(client);
	}else{
		client.loop();
	}
  now = millis();
  if (now - before > 1000) {
  	before = now;
  	if(st.AUTOMA){
  		readTemps();
  		controlHeat();
  	}
  	if(st.HAY_CNG){
      //console.log("example console.log entry");
  		publishState();
  		st.HAY_CNG=0;
  	}
  }	
}
#include "config.h"
#include <EEPROM.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "MQclient.h"

#define HOAH 14
#define HOAA 13
#define ALED 5
#define ONE_WIRE_BUS 4  // DS18B20 pin

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

WiFiClient espClient;
PubSubClient client(espClient);
Console console(devid, client);
MQclient mq(devid);


char topicin[16];
char incoming[80];
char cmd[20];

bool AUTOMA=1;
bool HAY_CNG=1;
bool NEW_MAIL=0;

struct STATE {
	int temp1=32;
	int temp2=40;
	bool heat=0;
	int hilimit=85;
	int lolimit=75;
} st;

void handleMqttIn(char* topic, byte* payload, unsigned int length) {
  for (int i=0;i<strlen(topic);i++) {
    topicin[i] = topic[i];
  }
  topicin[strlen(topic)] = '\0';  
  for (int i=0;i<length;i++) {
    incoming[i] = (char)payload[i];
  }
  incoming[length] = '\0';
  NEW_MAIL = 1;
}

void processIncoming(){
  Serial.println(topicin);
  Serial.println(incoming);
  StaticJsonBuffer<200> jsonBuffer;
  // "{\"heat\":1,\"src\":1,\"empty\":1}"
  JsonObject& root = jsonBuffer.parseObject(incoming);
  bool heat = root["heat"];
  bool automa = root["auto"];
  int hilimit = root["hilimit"];
  int lolimit = root["lolimit"];
  bool empty = root["empty"];
  if(heat != st.heat){
  	st.heat = heat;
    digitalWrite(ALED, st.heat);
    HAY_CNG=1;
  }
  if(automa != AUTOMA){
  	AUTOMA = automa;
    HAY_CNG=1;
  }
  if(hilimit > 0 && hilimit != st.hilimit){
  	st.hilimit = hilimit;
    HAY_CNG=1;
  }
  if(lolimit > 0 && lolimit != st.lolimit){
  	st.lolimit = lolimit;
    HAY_CNG=1;
  }
  if (empty==1){
  	eraseConfig();
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);  
    NEEDS_RESET=1;    
  }  
  NEW_MAIL=0;  
}


void publishState(){
	char astr[120];
	sprintf(astr, "{\"temp1\":%d, \"temp2\":%d, \"heat\":%d, \"hilimit\":%d, \"lolimit\":%d, \"auto\":%d  }", st.temp1, st.temp2, st.heat, st.hilimit, st.lolimit, AUTOMA);
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
		HAY_CNG=1;
	}
	if(temp2 != st.temp2){
		st.temp2=temp2;
		HAY_CNG=1;
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
		digitalWrite(ALED, st.heat);
		HAY_CNG= 1;
	}
}

// void reconn() {
//   Serial.print("Attempting MQTT connection...");
//   if (client.connect(devid)) {
//     Serial.println("connected");
//     client.subscribe(cmd);
//     return;
//   } else {
//     Serial.print("failed, rc=");
//     Serial.print(client.state());
//     delay(5000);
//     Serial.println(" try again in 5 seconds");
//   }
// }

void setup(){
	Serial.begin(115200);
	EEPROM.begin(512);
	Serial.println();
	Serial.println("--------------------------");
  Serial.println("ESP8266 mqttdemo");
  Serial.println("--------------------------");
  getOnline();
  strcpy(cmd, devid);
  strcat(cmd,"/cmd");
  client.setServer(ip, 1883);
	client.setCallback(handleMqttIn);  
	pinMode(HOAH, INPUT);//pullup
  pinMode(HOAA, INPUT);//pullup
  pinMode(ALED, OUTPUT);
  digitalWrite(ALED, st.heat);
}

long before = 0;
long now;

void loop(){
	server.handleClient();
	if(NEW_MAIL){processIncoming();}
	if(!client.connected() && !NEEDS_RESET){
		 mq.reconn(client);
	}else{
		client.loop();
	}
  now = millis();
  if (now - before > 1000) {
  	before = now;
  	if(AUTOMA){
  		readTemps();
  		controlHeat();
  	}
  	if(HAY_CNG){
  		publishState();
  		HAY_CNG=0;
  		console.log("example console.log entry");
  	}
  }	
}
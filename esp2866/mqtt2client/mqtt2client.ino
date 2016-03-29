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

bool AUTOMA=1;
bool HAY_CNG=1;

struct STATE {
	int temp1=32;
	int temp2=40;
	bool heat=0;
	int hilimit=85;
	int lolimit=75;
} st;

void handleMqttIn(char* topic, byte* payload, unsigned int length) {
  for (int i=0;i<strlen(topic);i++) {
    itopic[i] = topic[i];
  }
  itopic[strlen(topic)] = '\0';  
  for (int i=0;i<length;i++) {
    ipayload[i] = (char)payload[i];
  }
  ipayload[length] = '\0';
  NEW_MAIL = 1;
}

void processIncoming(){
  Serial.println(itopic);
  Serial.println(ipayload);
  StaticJsonBuffer<200> jsonBuffer;
  // "{\"heat\":1,\"src\":1,\"empty\":1}"
  JsonObject& root = jsonBuffer.parseObject(ipayload);
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

void do1thing(){
  Serial.println("do1thing");
}
void do2thing(){
  Serial.println("do2thing");
}
WiFiClient agetClient;
void getServerJSON(char* path){
  strcat(path,devid);
  Serial.println(path);
  if (!agetClient.connect(ip, atoi(port))) {
    Serial.println("connection failed");
  }
  agetClient.print(String("GET ") + path + " HTTP/1.1\r\n" +
               "Host: " + ip + "\r\n" + 
               "Connection: close\r\n\r\n");
  int timeout = millis() + 5000;
  while (agetClient.available() == 0) {
    if (timeout - millis() < 0) {
      agetClient.stop();
      Serial.println(">>> Client Timeout !");
    }
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
	pinMode(HOAH, INPUT);//pullup
  pinMode(HOAA, INPUT);//pullup
  pinMode(ALED, OUTPUT);
  digitalWrite(ALED, st.heat);
}

long before = 0;
long before10 = 0;
long before20 = 0;
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
  if (now - before10 > 10000) {
    before10 = now;
    do1thing();
    getServerJSON("/api/sched/senrel/");
  }
  if (now - before20 > 15000) {
    before20 = now;
    do2thing();
    getServerJSON("/api/sched/time/");
  }
  char jst[140];
  while(agetClient.available()){
    String line= agetClient.readStringUntil('\r'); 
    if(line.indexOf("{")!=-1 || line.indexOf("[")!=-1 ){
      strcpy(jst,line.c_str());
    }
  } 
  Serial.println(jst);    
  
}
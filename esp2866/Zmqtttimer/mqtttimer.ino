#include <EEPROM.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include "config.h"
#include "MQclient.h"
#include "ProgTimer.h"

#define HOAH 14
#define HOAA 13
#define ALED 5
#define ONE_WIRE_BUS 4  // DS18B20 pin

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

WiFiClient espClient;
WiFiClient tdClient;
WiFiClient dmClient;
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
  char hmns[6];
  sprintf(hmns, "%d:%d", hour(), minute());
  console.log(hmns);
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

const char* host = "128.138.141.172";
const int httpPort = 13;
String getTimeDate(){
  if (!espClient.connect(host, httpPort)) {
    Serial.println("connection failed");
    return "no data/time";
  }
  return espClient.readStringUntil('\r');
}

void getServerJSON(char* path, char* jst){
  strcat(path,devid);
  Serial.println(path);
  if (!espClient.connect(ip, atoi(port))) {
    Serial.println("connection failed");
  }
  espClient.print(String("GET ") + path + " HTTP/1.1\r\n" +
               "Host: " + ip + "\r\n" + 
               "Connection: close\r\n\r\n");
  int timeout = millis() + 5000;
  while (espClient.available() == 0) {
    if (timeout - millis() < 0) {
      espClient.stop();
      Serial.println(">>> Client Timeout !");
    }
  }
  while(espClient.available()){
    String line= espClient.readStringUntil('\r'); 
    if(line.indexOf("{")!=-1 || line.indexOf("[")!=-1 ){
      strcpy(jst,line.c_str());
    }
  }
  //espClient.stop();
} 

// char kstr[100]
// getServerJSON("/api/sched/time/", kstr);
// Serial.println(kstr);

struct TimeData {
  time_t unix;
  const char* LLLL;
  int zone;
};

bool deserialize(TimeData& data, char* kstr){
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(kstr);
  data.unix = root["unix"];
  data.LLLL = root["LLLL"];
  data.zone = root["zone"];
  return root.success();  
}

void setCloc(TimeData& data){
  Serial.println(data.unix); 
  time_t datetime = data.unix + data.zone*60*60;
  Serial.println(datetime); 
  setTime(datetime);
  setSyncInterval(4000000); 
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(" ");
  Serial.print(month());
  Serial.print("/");
  Serial.print(day());
  Serial.print("/");
  Serial.print(year());
  Serial.print(" ");
  Serial.print(weekday(datetime));
  Serial.println();
  // Serial.println(datetime);
  // Serial.println(now());
  // Serial.println(timeStatus());
  // Serial.println("closing connection");   
}



void getMyTime(){
  char url[50] = "/api/sched/time/";
  strcat(url,devid);
  console.log(url);
  if (!espClient.connect(ip, atoi(port))) {
    Serial.println("connection failed");
    return;
  }
  espClient.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + ip + "\r\n" + 
               "Connection: close\r\n\r\n");
  int timeout = millis() + 5000;
  while (espClient.available() == 0) {
    if (timeout - millis() < 0) {
      Serial.println(">>> Client Timeout !");
      espClient.stop();
      return;
    }
  }
  char jstr[100];
  while(espClient.available()){
    String line= espClient.readStringUntil('\r'); 
    //Serial.print(line); 
    if(line.indexOf("{")!=-1 || line.indexOf("[")!=-1 ){
      strcpy(jstr,line.c_str());
    }

  }
  Serial.println(jstr); 
  StaticJsonBuffer<200> tBuffer;
  JsonObject& root = tBuffer.parseObject(jstr);
  time_t unix = root["unix"];  
  int zone = root["zone"];
  time_t datetime = unix + zone*60*60;
  setTime(datetime);
  setSyncInterval(4000000); 
  Serial.print(hour(datetime));
  Serial.print(":");
  Serial.print(minute(datetime));
  Serial.print(" ");
  Serial.print(month(datetime));
  Serial.print("/");
  Serial.print(day(datetime));
  Serial.print("/");
  Serial.print(year(datetime));
  Serial.print(" ");
  Serial.print(weekday(datetime));
  Serial.println();
  Serial.println(datetime);
  Serial.println(now());
  Serial.println(timeStatus());
  Serial.println("closing connection");  
}

int aday[][3]= {{6,12,68},
          {8,20,57},
          {10,0,68},
          {11,30,58}
          };
int bday[][3]= {6,12,68,8,20,57,10,0,68,11,30,58};

void helpp(){
  Serial.println("ALARMALARM  ALEARM");
  Serial.println(aday[1][2]);
  Serial.println(aday[2][2]);
}

void getSched(){
  Serial.println("getting schedule");
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
  Serial.println(getTimeDate());
  //setCloc();
  //getMyTime();
  Alarm.alarmOnce(14, 52, 0, helpp);
  helpp();
  getSched();
  // char jso[100];
  // getServerJSON("/api/sched/senrel/", jso);
  // Serial.println(jso);
  // delay(2000);
  char kstr[100];
  getServerJSON("/api/sched/time/", kstr);
  Serial.println(kstr);
  TimeData data;
  deserialize(data, kstr);
  setCloc(data);
  Serial.println(data.unix);
  Serial.println(data.LLLL);
  Serial.println(data.zone);
}

long before = 0;
long noww;

void loop(){
  Alarm.delay(1000);
	server.handleClient();
	if(NEW_MAIL){processIncoming();}
	if(!client.connected() && !NEEDS_RESET){
		 mq.reconn(client);
	}else{
		client.loop();
	}
  noww = millis();
  if (noww - before > 1000) {
  	before = noww;
  	if(AUTOMA){
  		readTemps();
  		controlHeat();
  	}
  	if(HAY_CNG){
      console.log("about to publish");
  		publishState();
  		HAY_CNG=0;
  	}
  }	
}
#include <Arduino.h>
#include "MQclient.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

WiFiClient wfClient;
PubSubClient psclient(wfClient);

//MQclient::MQclient(PubSubClient psclient){
MQclient::MQclient(){
	sclient = psclient;
	//sclient.setCallback(this->handleCallback);
}

// void MQclient::handleCallback(char* topic, byte* payload, unsigned int length){
// 	Serial.println("handling callback");
// }
void handleCallback(char* topic, byte* payload, unsigned int length){
	Serial.println("handling callback");
}

void Console::log(char* dd){
	char log[20];
	strcpy(log,"CYURD001");
	strcat(log,"/log");
  if (psclient.connected()){
    psclient.publish(log, dd, true);
  }		
}
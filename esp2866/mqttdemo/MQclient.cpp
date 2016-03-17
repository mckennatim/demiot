#include "MQclient.h"
#include <Arduino.h>
#include <PubSubClient.h>

Console::Console(char* devid, PubSubClient& client ){
  cdevid = devid;
  cclient = client;
}

void Console::log(char* dd){
	char log[20];
	strcpy(log,cdevid);
	strcat(log,"/log");
  if (cclient.connected()){
    cclient.publish(log, dd, true);
  }		
}

MQclient::MQclient(char* devid){
	cdevid = devid;
}

void MQclient::reconn(PubSubClient& client) {
  Serial.print("Attempting remo MQTT connection...");
  if (client.connect(cdevid)) {
    Serial.println("connected");
    char ccmd[20];
    strcpy(ccmd, cdevid);
  	strcat(ccmd,"/cmd");
    client.subscribe(ccmd);
    return;
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    delay(5000);
    Serial.println(" try again in 5 seconds");
  }
}
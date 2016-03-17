#include "Console.h"
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
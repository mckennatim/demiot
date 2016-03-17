#ifndef MQclient_h
#define MQclient_h

#include <PubSubClient.h>

class Console{
public:
  Console(char* devid, PubSubClient& client);
  PubSubClient cclient;
  char* cdevid;
	void log(char* dd);
};

class MQclient{
public:
	MQclient(char* devid);
  char* cdevid;
	void reconn(PubSubClient& client);
};

#endif
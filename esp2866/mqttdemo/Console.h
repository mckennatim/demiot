#ifndef Console_h
#define Console_h

#include <PubSubClient.h>

class Console{
public:
  Console(char* devid, PubSubClient& client);
  PubSubClient cclient;
  char* cdevid;
	void log(char* dd);
};

#endif
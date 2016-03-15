#ifndef config_h
#define config_h
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

class Config{
private:
	ESP8266WebServer server;
	void scan();
	void setupAP();
public:
	void handleRoot();
	Config(ESP8266WebServer& server);
	void getOnline();
};


#endif
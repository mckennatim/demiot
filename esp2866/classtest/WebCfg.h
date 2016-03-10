#ifndef HEADER_WEBCFG
#define HEADER_WEBCFG

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

class WebCfg{
	public:
		WebCfg(int port);
		char ssids[300];
		void getSSIDs();
		void setupServer();
	private:		
		int _port;
		ESP8266WebServer server;
};
#endif

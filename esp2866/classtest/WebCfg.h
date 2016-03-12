#ifndef HEADER_WEBCFG
#define HEADER_WEBCFG

#include "Arduino.h"
//#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>

// void setupServer();
// ESP8266WebServer server(80);
// char ssid[40];
// char pwd[24];
// char devid[9];
// char ip[16];
// char port[5];

class WebCfg{
	public:
		WebCfg(int port);
		char ssids[300];
		void getSSIDs();
		// void startAp();
		//void setupServer();
		// //void serverBegin();
		// void saveCfg(char cf[]);
		// void showCfg();
		// ESP8266WebServer server;
		// char ssid[40];
		// char pwd[24];
		// char devid[9];
		// char ip[16];
		// char port[5];
	private:		
		int _port;
};
#endif

#include "Config.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

char *espssid = "espAPsb";
char *ssid = "street_no_vale2";
char *pwd = "jjjjjjjjx";
char ssids[300];

Config::Config(ESP8266WebServer& server){
  this->server = server;
}

void Config::handleRoot(){
	this->server.send(200, "text/html", "<h1>root of espAPsb AP server</h1>");
}

void Config::scan(){
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");  
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");  	
}

void Config::setupAP(){
	WiFi.softAP(espssid);
	this->server.on("/", handleRoot);
	this->server.begin();
  Serial.println();
  Serial.print("connected as AP ");
  Serial.println(espssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP()); 	
}

void Config::getOnline(){
	WiFi.begin(ssid, pwd);
  int tries =0;
  int success=1;
  while (WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
    tries++;
    if (tries==15){
      success=0;
      Serial.println("WiFi not connected");
      this->scan();
      this->setupAP();
      break;
    }
  }
  if (success){
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());    
  } 
}
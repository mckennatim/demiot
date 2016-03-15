#include "config.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>


char *espssid = "espAPsb";
char ssid[40] = "street_no_vale2";
char pwd[24] = "jjjjjjjjx";
char devid[9];
char ip[16];
char port[5];
char ssids[500];

ESP8266WebServer server(80);
EEPROM.begin(512);

// void saveCfg(char cf[])
// {
//   for (int i = 0; i < strlen(cf); ++i)
//   {
//     EEPROM.write(i, cf[i]);
//     Serial.print("Wrote: ");
//     Serial.println(cf[i]); 
//   }  
// }

void showCfg()
{
 Serial.println(ssid);
 Serial.println(pwd);
 Serial.println(devid);
 Serial.println(ip);
 Serial.println(port);
}

void handleRoot(){
  server.send(200, "text/html", ssids);
}
//http://192.168.4.1/config?ssid=street_no_vale2&pwd=jjjjjjjj&devid=CYURD001&ip=10.0.1.104&port=3332
void handleConfig(){
  strcpy(ssid, server.arg("ssid").c_str());
  strcpy(pwd, server.arg("pwd").c_str());
  strcpy(devid, server.arg("devid").c_str());
  strcpy(ip, server.arg("ip").c_str());
  strcpy(port, server.arg("port").c_str());
  server.send(200, "application/json", "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}");
  showCfg();
  // this->saveCfg(ssid);
}

void getSSIDs()
{
  char str[50];
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(200);  
  int n = WiFi.scanNetworks();
  Serial.print(n);
  Serial.println(" networks found"); 
  strcpy(ssids,"{\"ssids\": [");
  char tr[n][25];
  int tr2[n][2];
  for (int i = 0; i < n; ++i){
    strcpy(tr[i],WiFi.SSID(i).c_str());
    tr2[i][0]=WiFi.RSSI(i);
    tr2[i][1]=WiFi.encryptionType(i);
  }
  for (int i = 0; i < n; ++i){
    sprintf(str,"{\"ssid\": \"%s\", \"rssi\": %d, \"enc\": %d }", tr[i], tr2[i][0], tr2[i][1]);
    //sprintf(str,"[\"ssid\": \"%s\"]",  tr[i]);
    strcat(ssids,str);
    if(i<n-1){strcat(ssids, ", ");}
  }
  strcat(ssids,"]}");
}

// void getSSIDs(){
//   WiFi.mode(WIFI_STA);
//   WiFi.disconnect();
//   delay(100);
//   int n = WiFi.scanNetworks();
//   Serial.println("scan done");  
//   if (n == 0)
//     Serial.println("no networks found");
//   else
//   {
//     Serial.print(n);
//     Serial.println(" networks found");
//     for (int i = 0; i < n; ++i)
//     {
//       // Print SSID and RSSI for each network found
//       Serial.print(i + 1);
//       Serial.print(": ");
//       Serial.print(WiFi.SSID(i));
//       Serial.print(" (");
//       Serial.print(WiFi.RSSI(i));
//       Serial.print(")");
//       Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
//       delay(10);
//     }
//   }
//   Serial.println("");  	
// }

void setupAP(){
	WiFi.softAP(espssid);
  server.on("/", handleRoot);
  server.on("/config", handleConfig);
	server.begin();
  Serial.println();
  Serial.print("connected as AP ");
  Serial.println(espssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP()); 	
}

void getOnline(){
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
      getSSIDs();
      Serial.println(ssids);
      setupAP();
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
#include <ESP8266WiFi.h>
#include "helper.h"

//char str[50];
//char bigstr[400];
//int x = 14;



void setupAp(char* bigstr, int& blen){
  char str[50];
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);  
  int n = WiFi.scanNetworks();
  Serial.print(n);
  strcpy(bigstr,"");
  Serial.println(" networks found");
  for (int i = 0; i < n; ++i){
    strcpy(str,String(i + 1).c_str());
    strcat(str,": ");
    strcat(str,WiFi.SSID(i).c_str());
    strcat(str," (");
    strcat(str,String(WiFi.RSSI(i)).c_str());
    strcat(str,")");
    strcat(str, (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
    strcat(str,"\n");
    strcat(bigstr,str);
  } 
  //Serial.println(bigstr);
  blen = strlen(bigstr);
  // Serial.println(strlen(bigstr)); 
  // Serial.println(blen); 
  delay(10); 
  //return str;  
}


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("");
  Serial.println("--------------------------");
  Serial.println("ESP8266 Timer Test");
  Serial.println("--------------------------");
  timerCompleted = false;
  helper_function();
  user_init();
  char ssinf[400];
  int bl;
  setupAp(ssinf, bl);
  Serial.println(ssinf);
  Serial.println(bl);
}

void loop() {
  
  if (timerCompleted == true){
    Serial.println("Tick Occurred");
    timerCompleted = false;
  }
  //delay(5000);
  yield();  
}
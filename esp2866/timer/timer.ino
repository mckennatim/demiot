#include <ESP8266WiFi.h>
#include "helper.h"
#include "WifiCfg.h"

WifiCfg cfg;
//char str[50];
//char bigstr[400];
//int x = 14;

// void setupAp(char ss[15][50]){
//   WiFi.mode(WIFI_STA);
//   WiFi.disconnect();
//   delay(100);  
//   int n = WiFi.scanNetworks();  
// }


void setupAp(char* bigstr, int& blen){
  char str[50];
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);  
  int n = WiFi.scanNetworks();
  char* ssiStrings[n];
  char md[n][50];

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
    strcpy(md[i],str);
    strcat(str,"\n");
    strcat(bigstr,str);
  } 
  blen = strlen(bigstr);
  Serial.println(md[2]);
  delay(10); 
}

void reta(char da[15][50]){
  int n = 6;
  for(int i =0; i<n; i++){
    char s[50];
    strcpy(s,"this is number ");
    strcat(s, String(i).c_str());
    strcpy(da[i], s);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("");
  Serial.println("--------------------------");
  Serial.println("ESP8266 Timer Test");
  Serial.println("--------------------------");
  cfg.getSSIDs();
  timerCompleted = false;
  helper_function();
  user_init();
  char ssinf[400];
  int bl;
  setupAp(ssinf, bl);
  Serial.println(ssinf);
  Serial.println(bl);
  //Serial.println(md);
 
  char* myStrings[]={"This is string 1", "This is string 2", "This is string 3",
    "This is string 4", "This is string 5","This is string 6"};
  for (int i = 0; i < 6; i++){
    Serial.println(myStrings[i]);
    delay(500);
  }
  char a[6][20];
  for (int i = 0; i < 6; i++){
    char strr[20];
    strcpy(strr,"This is string ");
    strcat(strr, String(i+1).c_str());    
    strcpy(a[i], strr);
  }
  Serial.println(a[3]);
  char ma[15][50];
  reta(ma);
  for (int i = 0; i < 6; i++){
    Serial.println(ma[i]);
  }
}

void loop() {
  
  if (timerCompleted == true){
    Serial.println("Tick Occurred");
    timerCompleted = false;
  }
  //delay(5000);
  yield();  
}
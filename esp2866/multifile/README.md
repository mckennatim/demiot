
### Multi-file arduino esp8266 sketch sharing a reference to server
(transferred from stackoverflow on the advice of @Gee_Bee I can get https://www.arduino.cc/en/Hacking/LibraryTutorial to work as well as http://arduino.land/FAQ/content/7/43/en/breaking-a-sketch-into-multiple-files.html and http://arduinoetcetera.blogspot.com/2011/01/classes-within-classes-initialiser.html I can even get extern references to variables to work but referencing the an instance of ESP8266WebServer in different files is still beyond me)

As my sketches get bigger, the code looks awful as one file and the number of global variables is too high. 

For the webconfig part of my sketch the device attempts to get online and if it fails it scans for available wifi networks, goes into Access Point Mode, starts a server, sends the list of ssid's and listens for a response of an ssid/passwd pair. The server is only for the configuration.

Ideally I'd like the cleaned up main.ino file to look like

        #include "config.h"
        void setup(){
            Serial.begin(115200);
            Serial.println();
            Serial.println("--------------------------");
          Serial.println("ESP8266 multifile");
          Serial.println("--------------------------");
          getOnline();
        }
        void loop(){
            if(IN_CONFIG_MODE){
               server.handleClient();
            }
        }

with a config.h something like..

        #ifndef config_h
        #define config_h
            #include <ESP8266WiFi.h>
            #include <ESP8266WebServer.h>
            ESP8266WebServer server;
            void getOnline();
        #endif

and then a config.cpp where it does all stuff described above yet gives the main.ino program access to the `server` instance so it can listen in the main loop.

    #include "config.h"
    #include <Arduino.h>
    #include <ESP8266WiFi.h>
    #include <ESP8266WebServer.h>

    char *espssid = "espAPsb";
    char *ssid = "street_no_vale2";
    char *pwd = "jjjjjjjjx";
    char ssids[300];

    extern server(80);

    void handleRoot(){
        server.send(200, "text/html", "<h1>root of espAPsb AP server</h1>");
        //send json of available ssids
    }

    void scan(){
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

    void setupAP(){
        WiFi.softAP(espssid);
        server.on("/", handleRoot);
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
          scan();
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
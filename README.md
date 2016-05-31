#demiot
IOT demo project using Wemos ESP8266 running MQTT, a node express server, mqqt broker and websocket server, and clients of various flavors
 
 
##
### 41-move-server3-to-cloud
to Sitebuilt.net 162.217.250.109 /var/www/demiot/server3. Running in forever

    [
    ...
      {
        //demiot exp:3332, ws:3333, mqtt:1883
        "uid": "demiot",
        "append": true,
        "watch": true,
        "script": "index.js",
        "sourceDir": "/var/www/demiot/server3/lib"
      }
    ]

logging at `tail -f /root/.forever/demiot.log`

### 40-demo-sensor-timer-feature-summary
Oh shit what did I build?. What is actually in this demoiot platform and where is it programmed? on the device, on the server, on the raw client? Here are its modules.
* temp1: is all hooked up with a relay(heat), a hi&lo limit, a min freakin heating system. It runs on a schedule that gets uploaded from /client/raw as the first item in serels[0, 99, 1 ,2]. `raw` jerry-rigs it so it sends a schedule that has the esp8266 change limits starting in 1 minute and changing again 15 minutes after that. It operates from esp8266/sched.cpp/actProgs2 if((NEW_ALARM & 1) == 1) and tells the rest of the system that something happened by changing st.hi/lolimit and raising the HAY_CNG flag.
* temp2: reports its temperature only. It has no program serels[0, 99, 1 ,2], the 99 tells you so
* timr1: hard coded in esp8266/sched.cpp/actProgs2 if((NEW_ALARM & 4) == 4){ When true it sets it to false, sets dur=2 (minutes) then when it finishes and the callback `bm4` runs} `bm4` resets the mask so if((NEW_ALARM & 4) is true again which jumps back and resets the timer to 2 minutes again
* timr2: a programmable timer set by `raw` schedule button to start in a minute and run for 15. esp8266/sched.cpp/actProgs2 if((NEW_ALARM & 8) == 8 && senrels[3]<99) The prog actually tells it to be off at midnight then on and then off but nothing is implemented in the esp8266 to respond to that
* timr3: just like timr1 but set by esp8266/sched.cpp/actProgs2 if((NEW_ALARM & 16) == 16) to run for 1 minute then repeat

### 39-ngrx-store-select-async-json
So the asych data comes in on mqtt whenever it feels like and gets shepherded to the store. Meanwhile the container `mqtt.component.js` selects and subscribes to the store Observables `status` and `tmr` making any changes in that data available to any of the dumb components inside the container. NotWorking: <s>{{tmr.timr1 | async}}</s> {{(tmr | async).timr1}}
### 38-ngrx-store-dispatch-log
ng2 client now uses store, dispatching an action whenever `status` or `tmr` changes from mqtt message. The state object get replaced by the incoming message payload. Logging just works using  `ngrx-store-logger`. Back on the cutting edge baby.
### 37-ngrxrt
Added an angular2 client with @ngrx routing
### 36-time-alarms-mod
Changed the number of timers from default 6 to 12 in `#define dtNBR_ALARMS`. added a method `void TimeAlarms::clear()` in `/Documents/electronics/arduino/libraries/TimeAlarms-tim` to have `Alarm.clear()`called when we `proccessInc`oming progs.
### 35-bitmask-sched-timr
every time an alararm is done timr2 searches through the schedule until it goes past the current time. Calls that one the current schedule. Gets value from that and sets next alarm 
### 34-repeating-alarms
33-resetAlarms-actProgs was very buggy. Now in the middle of refactoring it, ie it is broken. Watch out and control NEW_ALARM. This is a bitmath version impllemented as `Sched::actProgs2` . Basically once you get to the end of that method NEW_ALARM goes to 0, waiting for the next alarm to go off. You have to wait for the time to be set however otherwise the alarms will be misset and nothing will ever happen. Time is requested at startup and then returns /progs so I wait for that to be done to set NEW_ALARM and actProgs2.
### 33-resetAlarms-actProgs
There is an individual callback for each of the programmable elements in schedArr[] When programs come over the wire (once a day probably, now implemented as a ui button that) schedutil.js get(s)Time to the device then after a second sendSchedule to the device. The device deseriProgs when they come in and then goes through al the incoming schedules and bootstraps them, setting alarms and device state based on the the current time and what's on the schedule. After that whenever an alarm finishes it sends a callback which sets a flag saying which alarm needs to be reset.
### 32-ArduinoJson
#### <a href="https://github.com/bblanchon/ArduinoJson/issues/251">parsing array of objects of multidimensional arrays</a>
My checklist for ArduinoJson
- make sure the json string is good and working in outside prog
- remember, you pass in a pointer to a char[] in deserialize. If you want to test inside the function use char[] for your test json
- use `DynamicJsonBuffer` until everything checks out 
- `prettyPrintTo(Serial)` is your friend, use it right away at the first parse

My test code below is full of all these uneccessary things that helped me understand this.

      int t0[4][3];
      int t1[4][3];

      bool Sched::deseriProgs(char* kstr){
        StaticJsonBuffer<600> jsonBuffer2;
        JsonArray& root = jsonBuffer2.parseArray(kstr);
        JsonArray& temp0 = root[0]["temp0"];
        JsonArray& temp1 = root[1]["temp1"];
        for(int i = 0; i<4; i++){
          for (int j = 0; j<3; j++){
            t0[i][j]=temp0[i][j];
          }
        }
        for(int i = 0; i<4; i++){
          for (int j = 0; j<3; j++){
            t1[i][j]=temp1[i][j];
          }
        }
        return root.success();
      }

      bool Sched::deseriProgs(char* kstr){
        Serial.println("in deseriprogs");
        char k[] = "[{\"temp0\":[[6,12,68],[8,20,57]]},{\"temp1\":[[6,0,67],[18,0,68],[21,30,58]]}]";
        StaticJsonBuffer<600> jsonBuffer2;
        //DynamicJsonBuffer jsonBuffer2;
        JsonArray& root = jsonBuffer2.parseArray(kstr);
        root.prettyPrintTo(Serial);
        JsonArray& temp0 = root[0]["temp0"];
        temp0.prettyPrintTo(Serial);
        JsonArray& temp1 = root[1]["temp1"];
        temp0[1][2].prettyPrintTo(Serial);
        Serial.print("the # of 3tuples is: ");
        Serial.println(temp0[0].size());
        Serial.println("temp0");  
        for(int i = 0; i<4; i++){
          Serial.print("[");
          for (int j = 0; j<3; j++){
            t0[i][j]=temp0[i][j];
            Serial.print(t0[i][j]);
            Serial.print(",");
          }
          Serial.print("]");
          Serial.println("");
        }
        Serial.println("temp1");  
        for(int i = 0; i<4; i++){
          Serial.print("[");
          for (int j = 0; j<3; j++){
            t1[i][j]=temp1[i][j];
            Serial.print(t1[i][j]);
            Serial.print(",");
          }
          Serial.print("]");
          Serial.println("");
        }
        return 1;
      }

      temp0
      [6,12,68,]
      [8,20,57,]
      [22,0,68,]
      [23,30,58,]
      temp1
      [6,0,67,]
      [9,20,57,]
      [18,0,68,]
      [21,30,58,]


Not sure if this type of JSON will work. If each object was just an array it will work but not if each object references a multidimensional array. 

    char* k = "[{\"temp0\":[[6,12,68],[8,20,57]]},{\"temp1\":[[6,0,67],[18,0,68],[21,30,58]]}]";
    StaticJsonBuffer<200> jsonBuffer;
    JsonArray& array = jsonBuffer.parseArray(k);
    array.prettyPrintTo(Serial); //[]
    Serial.println(array.size()); //0   
    JsonObject& temp0 = array[0]["temp0"];
    temp0.prettyPrintTo(Serial); //{}

btw: I love ArduinoJson. 

todo: get schedule with time
### 31-Sched-schedutil-mqtt2
Created a loop where a an alarm sets off a `Req`uest which is published to the server, picked up in `mqtt2`s `mq.selectAction` which then `mq.processIncoming` based on device and topic. In this case the topic is `time` and it fires us `schedutils` `getTime` which (will eventually goto Mongo to find the timezone for the device) and sends UTC time and the devices zone back on a `devtime` message. Since `MQclient::reconn` has the device `client.subscribe(devt);` , its `handleCallback` grabs the itopic and ipaylod and says `NEW_MAIL`. The device loop picks up that there is new mail and sorts the mail in `processInc` , instanting an `Sched` object to take care of it. Sched deserializes it to its json keys and then act `setClock` .
### 30-mqtall-Cmd.h
Each topic can have its own name. Each topics processing can now be handled by a separate class. caveat: handleCallback needs to be a function though I wanted it to be part of a class. It is a `functor` prograam. 

Re webconfig: the process is 
- disconnect after erase eeprom. 
- Wait, be sure espAP is not in the network list.
- connect and look for espAP to show up on the browser list, connect to that network
- browse 192.168.1.4
- rewrite eeprom with http://192.168.4.1/config?ssid=street_no_vale2&pwd=jjjjjjjj&devid=CYURD001&ip=10.0.1.104&port=3332
- disconnect device, wait
- reconnect and wait for action
### 29-mqttall
todo: classes instead of functions in c++
Tried to run a client as well as mqtt on the esp8266. Went fine for the first `get` request but crashed on the second. Wasted a lot of time and came up with nothing. In the process, got sick of the npm_module copy/delete in duplicating servers so moved all the npm_modules to `www/sbdevstack`. Now on server3 expanding lib/modules/mqqt.js to handle many possible inputs/outputs via their topics. Back on the esp8266, mqttall is being adapted as well.
### 28-timers
Can get current time on server with `moment().tz("America/New_York").format()` in a format `2016-03-24T12:48:07-04:00` .What would a timer want for format?
### 27-devices-db.spec.js
added lib/db/devices and tests for db
### 26-refactor-server2-modules
pulled out dattime and added routes to mqtt to display current message 
### 25-regtokau
moved in the token authentication module and its test suite, refactored it, went back to nodedemon to use es6 import/export
### 24-server2
Modularize server2, to be ready to add token auth and database for devices from www/node/sb/tokau-io-server and tmstack/token-auth-server
### 23-mqttTimer
getting date/time through nist string and by espClient get to server. OK problems are there is no clock on the esp8266 and millis only telly you the time since the last bootup, resetting at 50 days.DS3231 is rtc with alarm and tempperature.
### 22-handleCallback
Nothing much happens in a callback, it it fine to keep in MQclient along with the itopic and ipayload it produces ans well as NEW_MAIL.
### 21-MQclient-class
`MQclient` solves the main problem about passing classes by reference at the time of execution <s>(prior attempts initialized the class with the reference to other class)</s> by passing ext.class by reference to a method in another class. BTW console.log is ini tis own class in the same file.

    void MQclient::reconn(PubSubClient& client) {
      Serial.print("Attempting remo MQTT connection...");
      if (client.connect(cdevid)) {...
declared in .h as

    class MQclient{
    public:
      MQclient(char* devid);
      char* cdevid;
      void reconn(PubSubClient& client);
    };
then invoked `mq.reconn(client);`

### 20-Console-class
(Finally) created a class that includes a reference to a library instance.

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

      #include "Console.h"
      #include <Arduino.h>
      #include <PubSubClient.h>
      Console::Console(char* devid, PubSubClient& client ){
        cdevid = devid;
        cclient = client;
      }
      void Console::log(char* dd){
        char log[20];
        strcpy(log,cdevid);
        strcat(log,"/log");
        if (cclient.connected()){
          cclient.publish(log, dd, true);
        }   
      }

`Console console(devid,client)` intantiates console giving access to `devid` and `client` by passing it a pointer to the `devid` string and a reference to the `client`. Woo Woo. (this may be an innacurate description)



### 19-mqttdemo-1.0
refactored and running

<a href="https://github.com/Imroy/pubsubclient/issues/55">after webconfig state=-2 on reset</a>
### 18-EEPROM-save-read
in esp8266/multifile/config.cpp. Added functions for `showCfg()` , `server.on("/erase", handleErase);`, 

    void saveConfig(){
      for (int i = 0; i < 100; ++i) { EEPROM.write(i, 0); }
      idx[0]=saveCfg(ssid, 5);
      idx[1]=saveCfg(pwd, idx[0]);
      idx[2]=saveCfg(devid, idx[1]);
      idx[3]=saveCfg(ip, idx[2]);
      idx[4]=saveCfg(port, idx[3]);
      for (int i = 0; i < 5; ++i) { 
        EEPROM.write(i, idx[i]); 
      }
      EEPROM.commit();    
      showCfg();  
    }


The first 5 bytes of EEEPROM are the `uint8_t` indexes of each of the config variables. (written in the for loop above). `strlen` (from saveCfg) is where thos indexes come from.

    uint8_t saveCfg(char cf[], uint8_t start)
    {
      uint8_t fini = strlen(cf) + start;
      Serial.print("Wrote: ");
      Serial.print(fini);
      Serial.print(": ");
      for (int i = 0; i < strlen(cf); ++i){
        EEPROM.write(start+i, cf[i]);
        Serial.print(cf[i]); 
      } 
      Serial.println(); 
      return fini;
    }

Config variables are read out by first gettin the indexes and then the variables.

      void readConfig(){
        for (int i=0; i<5;i++){
          idx[i]=EEPROM.read(i);
          Serial.println(idx[i]);
        }
        getCfg(ssid, 5, idx[0]);
        getCfg(pwd, idx[0], idx[1]);
        getCfg(devid, idx[1], idx[2]);
        getCfg(ip, idx[2], idx[3]);
        getCfg(port, idx[3], idx[4]);
        showCfg();
      }

`char* s` is an out-parameter pointing to the first char address of whatever char[] you pass to it. 

    void getCfg(char* s, uint8_t beg, uint8_t end){
      for (int i=0;i<end-beg;i++){
        s[i]=char(EEPROM.read(i+beg));
      }
      s[end-beg]='\0';
    }


### 17-multifile-extern-server
<a href="https://github.com/esp8266/Arduino/issues/1776#issuecomment-196599815">esp8266/arduino issues</a>
 
However if you wanna have it nice, I'd make an own cpp class with the ESP8266 specific code, and would use that from my Arduino sketch. For example, your new cpp class could have one connect() method (aka getOnline) which you can nicely call from your main sketch. This is much better approach than letting the ESP8266WebServer instance being referred in all other souce codes - that would really break the concept of modular programming. – Gee Bee 4 hours ago
      
Yeah that's where I was heading. My problem was I still needed to use server,handleClient() in the main loop so I needed a reference to server. I suppose I could wrap that too inside a Config class and then just call cfg.getOnline() in setup and cfg.handleConfig() in loop. – mcktimo just now   edit   

<a href="http://stackoverflow.com/questions/35998907/multi-file-arduino-esp8266-sketch-sharing-a-reference-to-server?noredirect=1#comment59653729_35998907">Multi-file arduino esp8266 sketch sharing a reference to server</a>
Wow the world is changing. These days Github issues is so where it is at. extern ESP8266WebServer server; goes in the .h file and ESP8266WebServer server(80) goes in the .cpp file. It would be like that in c++ as well

in esp8266/multifile. Can now group functions in their own .h and .cpp files and have them share variables like a `ESP8266WebServer server(80);`  instance.
### 16-webconfig-basic
in esp2866/webconfig
### 15-recover
- `WiFi.softAP("esp8266sb", "jjjjjjjj", 6);` password needs to be at least 8 characters or it will just keep the previous one.
### 12-lambdas&initializers
oh my. 
<a href="http://arduinoetcetera.blogspot.com/2011/01/classes-within-classes-initialiser.html">classes within classes Christian Murphy</a>
#### iniitializers
If you create a class and you want to use another_class within it you have to initalize that another_class somehow to get at its methods. Here I want to use `ESP8266WebServer` in my `WebCfg` class. To do so you

- include other class in the header of your class
- declare it as a private variable to your class
- initalize it in the constructor of your .cpp 
- then use it at will
WebCfg.h


      #include <ESP8266WebServer.h>
      class WebCfg{
        public:
          ...
        private:    
          ESP8266WebServer server;

WebCfg.cpp

      WebCfg::WebCfg(int port): server(80)
      {
        _port=port;
      }

      ...
      void WebCfg::setupServer(){
        server.on("/", [&]() {
          IPAddress ip = WiFi.softAPIP();
          server.send(200, "text/html", ssids);  
        }); 
      }     
### lambdas 
Lambdas are like anonymous functions in javascript. They look like `[]() {}`
Problems like this happen `lambdas require capturing 'this' to call static member function?` The solution is to 'add the reference capture'. To do that you add `&` into the anony function like so `[&]() {}`

from @mk.
Lambda comes from the Lambda Calculus and refers to anonymous functions in programming. <a href="http://stackoverflow.com/questions/16501/what-is-a-lambda-function">stackoverflow @mk. Mikael Persson</a>

Why is this cool? It allows you to write quick throw away functions without naming them. It also provides a nice way to write closures. With that power you can do things like this.

python

```py
    def adder(x):
        return lambda y: x + y
    add5 = adder(5)
    add5(1) //6
```

As you can see from the snippet of Python, the function adder takes in an argument x, and returns an anonymous function, or lambda, that takes another argument y. That anonymous function allows you to create functions from functions. This is a simple example, but it should convey the power lambdas and closures have.



JavaScript

```js
    var adder = function (x) {
        return function (y) {
            return x + y;
        };
    };
    add5 = adder(5);
    add5(1) == 6
```

```js
var adder = x => y => x + y;
add5 = adder(5);
add5(1) == 6
```

Scheme

(define adder
    (lambda (x)
        (lambda (y)
           (+ x y))))
(define add5
    (adder 5))
(add5 1)
6
C# 3.5 or higher

Func<int, Func<int, int>> adder = 
    (int x) => (int y) => x + y; // `int` declarations optional
Func<int, int> add5 = adder(5);
var add6 = adder(6); // Using implicit typing
Debug.Assert(add5(1) == 6);
Debug.Assert(add6(-1) == 5);

// Closure example
int yEnclosed = 1;
Func<int, int> addWithClosure = 
    (x) => x + yEnclosed;
Debug.Assert(addWithClosure(2) == 3);
Swift

func adder(x: Int) -> (Int) -> Int{
   return { y in x + y }
}
let add5 = adder(5)
add5(1)
6
PHP

$a = 1;
$b = 2;

$lambda = function () use (&$a, &$b) {
    echo $a + $b;
};

echo $lambda();
Haskell

(\x y -> x + y) 

<ESP8266WebServer.h> needs <ESP8266WiFi.h>
### 11-classtest-WebCfg
Beginning of a class for webconfig of esp8288. Once you start `WiFi.scanNetworks()` you got to finish quick or crash. No time for sprintf. Put everything in temporary arrays and format the json string latter. Cool ssids is a class variable. Probably can be private. Don't need to carry it around (esp8266/classtest/classtest.ino)
### 10-classtest
esp8266/classtest/classtest.ino , You don't need a constructor <a href="https://www.arduino.cc/en/Hacking/LibraryTutorial">Arduino LibraryTutorial</a>
### 9-const-arrays-of-strings
#### How do you best construct an array of strings in c++ arduino (for esp8266)
in esp8266/timer/timer.ino
Say I have just created a char array `char strr[20]` and I want to add it to an array of char arrays `char bmyStrings[6][20];` 

If you knew what the strings were you could do it like 

    char* myStrings[]={"This is string 1", "This is string 2", "This is string 3","This is string 4", "This is string 5","This is string 6"};

But I don't. They need to be created on the fly. This works:

      char bmyStrings[6][20];  
      for(int i = 0; i < 6; ++i){
        char strr[20];
        strcpy(strr,"This is string ");
        strcat(strr, String(i+1).c_str(()));
        strcpy(bmyStrings[i],strr);
      } 


### 8-c++return-multiple-values
`void setupAp(char* bigstr, int& blen){}` returns to ssinf and bl...

      char ssinf[400];
      int bl;
      setupAp(ssinf, bl);
      Serial.println(ssinf);
      Serial.println(bl);

This is in `esp8266/timer/timer.ino setupAp()`
### 7-char[]-over-string
One should avoid the use of Strings in microcontroller, using char[] instead. To change string into a char array use `mystr.c_str()`. To convert an int to char array use `String(myint).c_str()` This is in `esp8266/timer/timer.ino setupAp()`
### 6-multiple-files-in-one-sketch
### Multiple Arduino Files in one sketch
The way that I use to create multiple Arduino files is the way that you would do that in c++ since I might as well learn the most general skill.

Basically you include a header.h in the ino, write a helper.h and helper.cpp and have them all in the same directory.

Each .cpp file needs an Arduino.h header

### 5-timer.ino

##### global variables
Variables shared between the .ino and .cpp that are needed for the .cpp are declared in the header and again in the .cpp as `extern`. then they are available in the .ino


### 4-refactorISR-mqttdemo
Make the interrupt for mqtt data as short as possible.

### 3-van-tempale2templString
Solved the problem of turning a nicely formatted  `<script type="text/content">` or `<template>` string with template variables into a template string via a generateTemplateString which creates a function that takes an object of template values and creates an HTML tring to be rendered.
### 2-van-rt-setContent
Routing now resets content to the innerHTML in `<script type="text/content" id="templ-empty">` . Can it set up listeners and use tempalte variables for props???
### 1-vanilla-navigo-webp
vanilla is an exploration on SPA without framework. I figure you need a router that on navigation calls a function that renders a page. So far Navigo loads with require (so I needed to install webpack)
### 0-initial commit
( to travel back in time `git stash` then `git checkout tagname`. once you are done then got back to your last commit `git checkout master` and add back the stuff you stashed `git stash apply`)
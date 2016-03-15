#demiot
IOT demo project using Wemos ESP8266 running MQTT, a node express server, mqqt broker and websocket server, and clients of various flavors


##
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
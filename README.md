#demiot
IOT demo project using Wemos ESP8266 running MQTT, a node express server, mqqt broker and websocket server, and clients of various flavors

##
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
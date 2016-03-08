#include "helper.h"

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
}

void loop() {
  if (timerCompleted == true){
    Serial.println("Tick Occurred");
    timerCompleted = false;
  }
  yield();  
}
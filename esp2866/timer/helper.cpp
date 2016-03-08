#include "Arduino.h"
#include "user_interface.h"
 
void helper_function( void ){
  Serial.println( "Called helper_function" );
}
os_timer_t myTimer;
extern bool timerCompleted;
void timerCallback(void *pArg) {
  timerCompleted = true;
}

void user_init(void) {
  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, 15000, true);//true-repeat
} 
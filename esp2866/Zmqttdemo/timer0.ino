
//#include <WiFiUdp.h>

// WiFiUDP Udp;
// unsigned int localPort = 8888;
// time_t getNtpTime();
// void digitalClockDisplay();
// Udp.begin(localPort);
os_timer_t myTimer;
bool timerCompleted;


void timerCallback(void *pArg) {
  timerCompleted = true;
}

void user_init(void) {
  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, 1000, true);//true-repeat
} 
//os_timer_disarm(&myTimer);


// void digitalClockDisplay(){
//   // digital clock display of the time
//   Serial.print(hour());
//   printDigits(minute());
//   printDigits(second());
//   Serial.print(" ");
//   Serial.print(day());
//   Serial.print(".");
//   Serial.print(month());
//   Serial.print(".");
//   Serial.print(year()); 
//   
// }
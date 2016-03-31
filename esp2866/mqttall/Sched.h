#ifndef Sched_h
#define Sched_h

#include "STATE.h"
#include <Arduino.h>


class Sched{
public:
	bool deserialize(char* kstr); 
	void act(STATE& st);
private:
  time_t unix;
  const char* LLLL;
  int zone;
};

#endif
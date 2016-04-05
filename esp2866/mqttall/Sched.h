#ifndef Sched_h
#define Sched_h

#include "STATE.h"
#include <Arduino.h>

void cbtmr1();
void cbtemp1();
extern int NEW_ALARM;

class Sched{
public:
	bool deserialize(char* kstr); 
	void actTime(STATE& st);
	bool deseriProgs(char* kstr); 
	//void actProgs(STATE& st);
	void bootstrapSched();
	void resetAlarm(int i, int &cur);
	void actProgs(int idx, int cur, STATE& st );
	int idxOsenrels(int j);
	//allocate for 8 sensor/relays with 6 scheduled events/day and 
	// hr, min + 2 settings they can affect
	int nsr; //number of programs
	int seresz; //sizeof senrels
	int progs[8][6][4]; //max 8 programs, w 6 events and 4 things set
	int senrels[8];//[0,99,1]
	int events[6];
	int haynRset[8];
private:
  time_t unix;
  const char* LLLL;
  int zone;
};

#endif
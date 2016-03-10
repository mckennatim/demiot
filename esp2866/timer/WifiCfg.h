#ifndef HEADER_WIFICFG
#define HEADER_WIFICFG

#include "Arduino.h"

class WifiCfg{
	public:
		WifiCfg();
		char ssids[300];
		void getSSIDs();
};
#endif
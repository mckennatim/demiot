#ifndef HEADER_WIFICFG
#define HEADER_WIFICFG

#include "Arduino.h"

class WifiCfg{
	public:
		char ssids[300];
		void getSSIDs();
};
#endif
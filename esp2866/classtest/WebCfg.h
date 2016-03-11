#ifndef HEADER_WIFICFG
#define HEADER_WIFICFG

#include "Arduino.h"

class WebCfg{
	public:
		char ssids[300];
		void getSSIDs();
};
#endif

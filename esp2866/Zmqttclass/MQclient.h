#ifndef MQclient_h
#define MQclient_h

	#include <PubSubClient.h>
	#define MQTT_CALLBACK_SIGNATURE std::function<void(char*, uint8_t*, unsigned int)> callback

	void handleCallback(char* topic, byte* payload, unsigned int length);

	class MQclient{
	public:
		MQclient();
		//MQclient(PubSubClient psclient);

		PubSubClient sclient;
		// PubSubClient& setCallback(MQTT_CALLBACK_SIGNATURE);
		//void handleCallback(char* topic, byte* payload, unsigned int length);
	};

class Console{
	public:
		void log(char* dd);
};

#endif
#ifndef _MQTTCLIENT_H_
#define _MQTTCLIENT_H_

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <MDebug.h>

extern void setup_mqttclient();
extern void loop_mqttclient();

#endif //_MQTTCLIENT_H_

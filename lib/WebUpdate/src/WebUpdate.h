#ifndef _WEBUPDATE_H_
#define _WEBUPDATE_H_

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

extern void setup_webupdate();
extern void loop_webupdate();

#endif //_WEBUPDATE_H_

#ifndef _SYNCTIME_H_
#define _SYNCTIME_H_

#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <NtpClientLib.h>

extern void setup_synctime();
extern void loop_synctime();

#endif //_SYNCTIME_H_

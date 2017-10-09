#ifndef _LEDSTRIP_H_
#define _LEDSTRIP_H_

#include <ESP8266WiFi.h>
#include <MDebug.h>
#include <Adafruit_NeoPixel.h>

extern void setup_ledstrip();
extern void loop_ledstrip();
extern void ledstrip_start_alarm();
extern void ledstrip_set_color(uint8_t r, uint8_t g, uint8_t b);

#endif //_LEDSTRIP_H_

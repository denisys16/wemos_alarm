#include <ESP8266WiFi.h>
#include <WebUpdate.h>
#include <LEDStrip.h>
#include <MQTTClient.h>
#include <SyncTime.h>
#include <MDebug.h>
#include <LEDStrip.h>

#define DEBUG_LOG_ENABLED 0

#if (DEBUG_LOG_ENABLED)
  #define DEBUGLOG(...) Serial.printf(__VA_ARGS__)
#else
  #define DEBUGLOG(...)
#endif

#include "passwd.h"
//const char* ssid = "SSID_NAME";
//const char* password = "PASSWORD";

void led_on()
{
  digitalWrite(LED_BUILTIN, LOW); // turn the LED ON (LOW is the voltage level)
}

void led_off()
{
  digitalWrite(LED_BUILTIN, HIGH); // turn the LED OFF (HIGH is the voltage level)
}

void led_blink()
{
  static long currTime = millis();
  static bool blinkState = true;
  static long next_delay = 100;
  if ((millis() - currTime) > next_delay)
  {

    if (blinkState)
    {
      next_delay = 80;
      led_on();
    }
    else
    {
      next_delay = 3000;
      led_off();
    }
    blinkState = !blinkState;
    currTime = millis();
  }
}

void setup_led()
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  led_off();
}

void setup_serial()
{
  #if (DEBUG_LOG_ENABLED)
    // Setup console
    Serial.begin(115200);
    delay(10);
    Serial.println();
    Serial.println();
  #endif
}

void wifi_connect()
{
  DEBUGLOG("Connecting to ");
  DEBUGLOG(ssid);
  DEBUGLOG("...");
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    DEBUGLOG("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }
  DEBUGLOG("WiFi connected");
}

void setup()
{
  rst_info *resetInfo;
  resetInfo = ESP.getResetInfoPtr();

  setup_serial();
  setup_mdebug();

  DEBUGLOG(ESP.getResetReason());
  DEBUGLOG("---");

  setup_led();
  setup_ledstrip();
  wifi_connect();
  setup_webupdate();
  setup_mqttclient();
  //setup_synctime();
}

uint32_t mTimeSeconds = 0;
void loop()
{
  loop_mdebug();
  if (WiFi.status() == WL_CONNECTED)
  {
    //led_blink();        yield();
    loop_webupdate();   yield();
    loop_mqttclient();  yield();
    loop_ledstrip();    yield();
    //loop_synctime();    yield();
  }
  else
  {
    led_off();
    DEBUGLOG("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }
}

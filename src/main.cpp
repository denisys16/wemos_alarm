#include <ESP8266WiFi.h>
#include <WebUpdate.h>

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
  static bool blinkState = false;
  if ((millis() - currTime) > 500)
  {
    blinkState = !blinkState;
    if (blinkState)
      led_on();
    else
      led_off();
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
  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
}

void wifi_connect()
{
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println("...");
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }
  Serial.println("WiFi connected");
}

void setup()
{
    setup_serial();
    setup_led();
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    led_off();
    wifi_connect();
    delay(1000);
    webupdate_connect();
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    led_blink();
    loop_webupdate();
  }
}

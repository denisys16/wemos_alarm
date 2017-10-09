#include <MQTTClient.h>
#include <LEDStrip.h>

const char* mqtt_client_name = "ESP8266WemosN1";
const char* topic_set_color = "wemos/rgb_color";
const char* topic_start_alarm = "alarm/start";

static WiFiClient wclient;
static IPAddress server_ip(192, 168, 1, 108);// Update these with values suitable for your network.
static void mqtt_callback(char* topic, byte* payload, unsigned int length);// Callback function header
static PubSubClient mqtt_client(server_ip, 1883, mqtt_callback, wclient);

void setup_mqttclient()
{

}

void loop_mqttclient()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        if (mqtt_client.connected())
        {
            mqtt_client.loop();
        }
        else
        {
            if (mqtt_client.connect(mqtt_client_name))
            {
                mqtt_client.subscribe(topic_set_color);
                mqtt_client.subscribe(topic_start_alarm);
            }
        }
    }
}

static void set_color(char *payload)
{
    Serial.println("LEDStrip set color called.");
    String payload_str((char *)payload);

    int c1 = payload_str.indexOf(';');
    int c2 = payload_str.indexOf(';',c1+1);

    int red = payload_str.toInt();
    int green = payload_str.substring(c1+1,c2).toInt();
    int blue = payload_str.substring(c2+1).toInt();

    ledstrip_set_color(red, green, blue);

    mdebug_printf(MLVL_DEBUG,"Set LED color to R:%u G:%u B:%u\n", red, green, blue);
}

static void start_alarm(char *payload)
{
    mdebug_printf(MLVL_DEBUG,"LEDStrip set alarm called.\n");
    ledstrip_start_alarm();
}

static void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    // Allocate the correct amount of memory for the payload copy
    byte* p = (byte*)malloc(length+1); // Add more space for convert to zero-terminated strings
    // Copy the payload to the new buffer
    memcpy(p,payload,length);
    p[length] = 0; // For zero-terminated strings

    mdebug_printf(MLVL_INFO,"MQTT: %s => %s\n", topic, p);

    if (String(topic) == topic_set_color)
        set_color((char *)p);
    else if (String(topic) == topic_start_alarm)
        start_alarm((char *)p);

  // Free the memory
  free(p);
}

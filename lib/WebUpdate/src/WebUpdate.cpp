/*
  To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/

#include <WebUpdate.h>

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup_webupdate()
{
    httpUpdater.setup(&httpServer);
    httpServer.begin();

    IPAddress ip = WiFi.localIP();
    Serial.printf("HTTPUpdateServer ready! Open http://%d.%d.%d.%d/update in your browser\n", ip[0], ip[1], ip[2], ip[3]);
}

void loop_webupdate()
{
    httpServer.handleClient();
}

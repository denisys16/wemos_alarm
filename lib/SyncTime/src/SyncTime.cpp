#include <SyncTime.h>

static void processSyncEvent(NTPSyncEvent_t ntpEvent) {
    if (ntpEvent) {
        Serial.print("Time Sync error: ");
        if (ntpEvent == noResponse)
            Serial.println("NTP server not reachable");
        else if (ntpEvent == invalidAddress)
            Serial.println("Invalid NTP server address");
    }
    else {
        mdebug_printf(MLVL_INFO, "Got NTP time: %s.", NTP.getTimeDateString(NTP.getLastNTPSync()).c_str());
    }
}

boolean syncEventTriggered = false; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent; // Last triggered event

void setup_synctime()
{
    NTP.onNTPSyncEvent([](NTPSyncEvent_t event) {
        ntpEvent = event;
        syncEventTriggered = true;
    });
    NTP.begin("192.168.1.1", 3, false);
}

void loop_synctime()
{
    static int last = 0;

    if (syncEventTriggered)
    {
        processSyncEvent(ntpEvent);
        syncEventTriggered = false;
    }

    if ((millis() - last) > 30000) // Every 30 seconds
    {
        last = millis();
        mdebug_printf(MLVL_VERBOSE, "SyncTime: %s - %s. Uptime: %s since %s.\n",
            NTP.getTimeDateString().c_str(),
            NTP.isSummerTime() ? "Summer Time" : "Winter Time",
            NTP.getUptimeString().c_str(),
            NTP.getTimeDateString(NTP.getFirstSync()).c_str());
    }
}

#include <MDebug.h>

#if (DEBUG_ENABLED)
    RemoteDebug Debug;

    void setup_mdebug()
    {
        // Initialize the telnet server of RemoteDebug
        Debug.begin("Telnet_Debug"); // Initiaze the telnet server
        Debug.setResetCmdEnabled(true); // Enable the reset command
        Debug.showTime(true); // To show time
        Debug.showProfiler(true); // To show profiler - time between messages of Debug
        Debug.setSerialEnabled(true);// All messages too send to serial too, and can be see in serial monitor
        //Debug.showColors(true); // Colors
    }

    void loop_mdebug()
    {
        Debug.handle();
    }

#endif


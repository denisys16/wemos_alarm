#ifndef _MDEBUG_H_
#define _MDEBUG_H_

#include <ESP8266WiFi.h>
#include <RemoteDebug.h>

extern RemoteDebug Debug;

#define MLVL_VERBOSE    (Debug.VERBOSE  )
#define MLVL_DEBUG      (Debug.DEBUG    )
#define MLVL_INFO       (Debug.INFO     )
#define MLVL_WARNING    (Debug.WARNING  )
#define MLVL_ERROR      (Debug.ERROR    )

#define mdebug_printf(level, ...) do {  \
    if (Debug.isActive(level))          \
        Debug.printf(__VA_ARGS__);      \
    } while(0)


extern void setup_mdebug();
extern void loop_mdebug();

#endif //_MDEBUG_H_

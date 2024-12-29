#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include "Pump.h"
#include "SSR.h"
#include "Sensors.h"
#include "Pause.h"

class SystemManager {
private:
    Pump pump;
    SSR ssr;
    Sensors sensors;
    Pause pauses[10];
    int pauseCount;
    bool isAutomatic;
    String brewStatus;

    unsigned long lastBroadcastTime;
    unsigned long broadcastInterval;

    static SystemManager* instance;

public:
    SystemManager();

    void init();
    void handleWebSocket();
    void handleWebSocketMessage(const String& message);
    void handlePeriodicTasks();
    String getStateJSON();

    void setPauseCount(int count);
    void addPause(const Pause& pause);
    void removeLastPause();
    void updatePause(int index, const Pause& pause);

    void setAutomaticMode(bool mode);
    bool getAutomaticMode() const;

    void setBrewStatus(const String& status);
    String getBrewStatus() const;            

    static void staticWebSocketHandler(const String& message);
};

#endif // SYSTEM_MANAGER_H

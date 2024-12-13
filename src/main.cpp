#include <WiFi.h>
#include "pauses.h"
#include "sensors.h"
#include "control.h"
#include "websocket.h"

// Wi-Fi credentials
const char* ssid = "agent_007 2G";
const char* password = "24031996";

void setup() {
    Serial.begin(115200);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }

    // Initialize modules
    initSensors();
    initControl();
    initPauses();
    initWebSocket();
}

void loop() {
    handleWebSocket();
    static unsigned long lastSendTime = 0;
    if (millis() - lastSendTime > 5000) {
        lastSendTime = millis();
        sendDebugInfo();
    }
}

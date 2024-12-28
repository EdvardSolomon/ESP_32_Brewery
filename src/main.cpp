#include <WiFi.h>
#include "SystemManager.h"

SystemManager systemManager;

void setup() {
    Serial.begin(115200);

    Serial.println("Подключение к Wi-Fi...");
    WiFi.begin("59", "universal");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi подключен");
    Serial.print("IP адрес: ");
    Serial.println(WiFi.localIP());

    systemManager.init();
}

void loop() {
    systemManager.handleWebSocket();
    systemManager.handlePeriodicTasks();
}

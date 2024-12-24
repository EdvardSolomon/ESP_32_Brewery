#include <WiFi.h>
#include <ArduinoJson.h>
#include "websocket.h"
#include "pauses.h"
#include "sensors.h"
#include "control.h"

void handleMessage(const String& message) {
    if (message.startsWith("SET_PAUSE_COUNT:")) {
        int count = message.substring(16).toInt();
        setPauseCount(count); // Используем функцию вместо прямого доступа к переменной
    } else if (message.startsWith("UPDATE_PAUSE:")) {
        int index, time;
        float temperature, hysteresis;
        sscanf(message.c_str(), "UPDATE_PAUSE:%d:%f:%f:%d", &index, &temperature, &hysteresis, &time);
        updatePause(index - 1, temperature, hysteresis, time); // Используем функцию
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.begin("59", "universal");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi подключен");
    Serial.print("IP адрес: ");
    Serial.println(WiFi.localIP());

    initPauses();
    initSensors();
    initWebSocket(handleMessage);
}


void loop() {
    handleWebSocket(); // Обрабатывает WebSocket-события

    // Отправка данных раз в секунду
    static unsigned long lastSendTime = 0;
    if (millis() - lastSendTime > 5000) {
        lastSendTime = millis();

        // Создайте JSON-документ с заранее определенным размером
        StaticJsonDocument<1024> doc; 

        unsigned long start = millis();
        appendSensorsInfoToJSON(doc);
        unsigned long sensorsTime = millis();
        Serial.println("Sensors JSON time: " + String(sensorsTime - start) + "ms");

        start = millis();
        appendPausesInfoToJSON(doc);
        unsigned long pausesTime = millis();
        Serial.println("Pauses JSON time: " + String(pausesTime - start) + "ms");

        start = millis();
        appendControlInfoToJSON(doc);
        unsigned long controlTime = millis();
        Serial.println("Control JSON time: " + String(controlTime - start) + "ms");

        // Преобразуйте JSON-документ в строку
        String debugInfo;
        serializeJson(doc, debugInfo);
        
        unsigned long jsonTime = millis();
        Serial.println("JSON generation time: " + String(jsonTime - lastSendTime) + "ms");

        sendMessageToAll(debugInfo);

        unsigned long sendTime = millis();
        Serial.println("Send time: " + String(sendTime - jsonTime) + "ms");
    }
}



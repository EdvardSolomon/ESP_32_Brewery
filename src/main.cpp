#include <WiFi.h>
#include "pauses.h"
#include "sensors.h"
#include "control.h"
#include "websocket.h"

// Wi-Fi credentials
const char* ssid = "59";
const char* password = "universal";

// Определение пина встроенного светодиода
#define LED_PIN 2 // На ESP32 встроенный светодиод обычно подключен к GPIO2

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT); // Установить светодиод как выход

    // Попытка подключения к Wi-Fi
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        // Светодиод мигает, пока нет подключения
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
        delay(500);
    }
    Serial.println("Wi-Fi подключен!");
    Serial.print("IP адрес: ");
    Serial.println(WiFi.localIP());
    // Если подключение установлено, выключить светодиод
    digitalWrite(LED_PIN, LOW);
}

void loop() {
    // Если подключение потеряно, светодиод начнет мигать
    if (WiFi.status() != WL_CONNECTED) {
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
        delay(100);
    } else {
        digitalWrite(LED_PIN, LOW); // Светодиод выключен при подключении
    }
}

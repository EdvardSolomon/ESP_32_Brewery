#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "Sensors.h"

// DS18B20 подключен к пину 15
#define ONE_WIRE_BUS 15
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensorBus(&oneWire);

Sensors::Sensors() : lastRequestTime(0) {
    for (int i = 0; i < 2; i++) {
        temperatures[i] = NAN; // Изначально температуры неизвестны
        addresses[i] = "not_found";
    }
}

void Sensors::init() {
    sensorBus.begin();

    // Сохраняем адреса сенсоров
    DeviceAddress tempAddress;
    for (int i = 0; i < 2; i++) {
        if (sensorBus.getAddress(tempAddress, i)) {
            char addr[17];
            for (uint8_t j = 0; j < 8; j++) {
                sprintf(&addr[j * 2], "%02X", tempAddress[j]);
            }
            addresses[i] = String(addr);
        } else {
            addresses[i] = "not_found";
        }
    }

    Serial.println("Sensors initialized");
}

void Sensors::requestTemperatures() {
    sensorBus.requestTemperatures();
    lastRequestTime = millis();
}

float Sensors::getTemperature(int index) {
    if (index < 0 || index >= 2) return -999;

    if (millis() - lastRequestTime >= 750) {
        DeviceAddress tempAddress;
        if (sensorBus.getAddress(tempAddress, index)) {
            temperatures[index] = sensorBus.getTempC(tempAddress);
            if (temperatures[index] == DEVICE_DISCONNECTED_C) {
                temperatures[index] = -999;
            }
        } else {
            temperatures[index] = -999;
        }
    }

    return temperatures[index];
}

String Sensors::getAddress(int index) {
    if (index < 0 || index >= 2) return "invalid_index";
    return addresses[index];
}

void Sensors::appendToJSON(JsonObject& doc) {
    JsonArray sensorsArray = doc.createNestedArray("sensors");

    for (int i = 0; i < 2; i++) {
        JsonObject sensorData = sensorsArray.createNestedObject();
        sensorData["index"] = i;

        float temperature = getTemperature(i);
        if (temperature == -999) {
            sensorData["temperature"] = "Not Found";
        } else {
            sensorData["temperature"] = temperature;
        }

        sensorData["address"] = getAddress(i);
    }
}

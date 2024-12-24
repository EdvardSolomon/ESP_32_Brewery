#include <Arduino.h>
#include "sensors.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// DS18B20
#define ONE_WIRE_BUS 15
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1, sensor2;

unsigned long lastTemperatureRequest = 0;  // Время последнего запроса температуры

void initSensors() {
    sensors.begin();
    sensors.getAddress(sensor1, 0);
    sensors.getAddress(sensor2, 1);
}

void requestTemperature() {
    sensors.requestTemperatures();  // Отправляем запрос на измерение температуры
    lastTemperatureRequest = millis();
}

float getTemperatureNonBlocking(int sensorIndex) {
    // Проверяем, прошло ли достаточно времени (750 мс для DS18B20)
    if (millis() - lastTemperatureRequest < 750) {
        return NAN;  // Данные еще не готовы
    }

    // Читаем температуру с датчиков
    if (sensorIndex == 0 && sensors.getAddress(sensor1, 0)) {
        return sensors.getTempC(sensor1);
    } else if (sensorIndex == 1 && sensors.getAddress(sensor2, 1)) {
        return sensors.getTempC(sensor2);
    }

    return NAN;
}

String getSensorAddress(int sensorIndex) {
    DeviceAddress address;
    if (sensorIndex == 0 && sensors.getAddress(sensor1, 0)) {
        memcpy(address, sensor1, sizeof(DeviceAddress)); // Копирование массива
    } else if (sensorIndex == 1 && sensors.getAddress(sensor2, 1)) {
        memcpy(address, sensor2, sizeof(DeviceAddress)); // Копирование массива
    } else {
        return "not_found";
    }

    String addr = "";
    for (uint8_t i = 0; i < 8; i++) {
        if (address[i] < 16) addr += "0";
        addr += String(address[i], HEX);
    }
    return addr;
}

void appendSensorsInfoToJSON(JsonDocument& doc) {
    JsonArray sensorsArray = doc.createNestedArray("sensors");

    for (int i = 0; i < 2; i++) {
        JsonObject sensorData = sensorsArray.createNestedObject();
        sensorData["index"] = i;
        sensorData["temperature"] = getTemperatureNonBlocking(i);  // Используем асинхронное чтение
        sensorData["address"] = getSensorAddress(i);
    }
}

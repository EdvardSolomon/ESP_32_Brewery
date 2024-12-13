#include <Arduino.h>
#include "sensors.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// DS18B20
#define ONE_WIRE_BUS 15
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1, sensor2;

void initSensors() {
    sensors.begin();
    sensors.getAddress(sensor1, 0);
    sensors.getAddress(sensor2, 1);
}

float getTemperature(int sensorIndex) {
    sensors.requestTemperatures();
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

#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <ArduinoJson.h>

void initSensors();
void requestTemperature();  // Асинхронный запрос температуры
float getTemperatureNonBlocking(int sensorIndex);
String getSensorAddress(int sensorIndex);
void appendSensorsInfoToJSON(JsonDocument& doc);

#endif // SENSORS_H

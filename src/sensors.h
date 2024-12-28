#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <ArduinoJson.h>

class Sensors {
private:
    float temperatures[2];
    String addresses[2];
    unsigned long lastRequestTime;

public:
    Sensors();

    void init();
    void requestTemperatures();       // Асинхронный запрос температуры
    float getTemperature(int index);
    String getAddress(int index);
    void appendToJSON(JsonObject& doc);
};

#endif // SENSORS_H

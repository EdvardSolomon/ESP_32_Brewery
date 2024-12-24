#ifndef PAUSES_H
#define PAUSES_H

#include <Arduino.h>
#include <ArduinoJson.h>

struct Pause {
    float temperature;
    float hysteresis;
    int time;
};

void initPauses();
void setPauseCount(int count);
void updatePause(int index, float temperature, float hysteresis, int time);

// Новая функция для добавления информации о паузах в JSON-документ
void appendPausesInfoToJSON(JsonDocument& doc);

#endif // PAUSES_H

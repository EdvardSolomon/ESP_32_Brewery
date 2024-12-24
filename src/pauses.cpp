#include "pauses.h"
#include <Arduino.h>

Pause pauses[10];
int pauseCount = 0;

void initPauses() {
    pauseCount = 0; // Инициализация
}

void setPauseCount(int count) {
    if (count >= 0 && count <= 10) {
        pauseCount = count;
    }
}

void updatePause(int index, float temperature, float hysteresis, int time) {
    if (index >= 0 && index < pauseCount) {
        pauses[index] = {temperature, hysteresis, time};
    }
}

void appendPausesInfoToJSON(JsonDocument& doc) {
    JsonArray pausesArray = doc.createNestedArray("pauses");

    for (int i = 0; i < pauseCount; i++) {
        JsonObject pauseData = pausesArray.createNestedObject();
        pauseData["index"] = i + 1; // Индексы начинаются с 1
        pauseData["temperature"] = pauses[i].temperature;
        pauseData["hysteresis"] = pauses[i].hysteresis;
        pauseData["time"] = pauses[i].time;
    }
}

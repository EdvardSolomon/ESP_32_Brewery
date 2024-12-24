#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include <ArduinoJson.h>

void initControl();
void togglePump();
void setPumpPWMLevel(int level);
void toggleSSR();
void setSSRPWMLevel(int level);

// Новая функция для добавления информации о контроле в JSON-документ
void appendControlInfoToJSON(JsonDocument& doc);

#endif // CONTROL_H

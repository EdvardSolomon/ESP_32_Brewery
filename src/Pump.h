#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>

class Pump {
private:
    bool enabled;
    int pwmLevel;
    int controlPin;
    int pwmPin;

public:
    Pump(); // Конструктор

    void init(int controlPin, int pwmPin);
    void setState(bool state);
    void setPWM(int level);
    bool isEnabled() const;
    int getPWMLevel() const;
};

#endif // PUMP_H

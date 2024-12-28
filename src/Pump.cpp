#include "Pump.h"

Pump::Pump() : enabled(false), pwmLevel(0), controlPin(-1), pwmPin(-1) {}

void Pump::init(int controlPin, int pwmPin) {
    this->controlPin = controlPin;
    this->pwmPin = pwmPin;
    pinMode(controlPin, OUTPUT);
    pinMode(pwmPin, OUTPUT);
    digitalWrite(controlPin, LOW);
    analogWrite(pwmPin, 0);
}

void Pump::setState(bool state) {
    enabled = state;
    digitalWrite(controlPin, enabled ? HIGH : LOW);
}

void Pump::setPWM(int level) {
    // Преобразуем уровень мощности из 0–100% в 0–255
    pwmLevel = constrain(map(level, 0, 100, 0, 255), 0, 255);
    analogWrite(pwmPin, pwmLevel);
}

bool Pump::isEnabled() const {
    return enabled;
}

int Pump::getPWMLevel() const {
    return pwmLevel;
}

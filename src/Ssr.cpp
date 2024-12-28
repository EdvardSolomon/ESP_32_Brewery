#include "SSR.h"

SSR::SSR() : enabled(false), pwmLevel(0), controlPin(-1), pwmPin(-1) {}

void SSR::init(int controlPin, int pwmPin) {
    this->controlPin = controlPin;
    this->pwmPin = pwmPin;
    pinMode(controlPin, OUTPUT);
    pinMode(pwmPin, OUTPUT);
    digitalWrite(controlPin, LOW);
    analogWrite(pwmPin, 0);
}

void SSR::setState(bool state) {
    enabled = state;
    digitalWrite(controlPin, enabled ? HIGH : LOW);
}

void SSR::setPWM(int level) {
    // Преобразуем уровень мощности из 0–100% в 0–255
    pwmLevel = constrain(map(level, 0, 100, 0, 255), 0, 255);
    analogWrite(pwmPin, pwmLevel);
}

bool SSR::isEnabled() const {
    return enabled;
}

int SSR::getPWMLevel() const {
    return pwmLevel;
}

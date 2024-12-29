#include "SSR.h"

SSR::SSR() : enabled(false), pwmLevel(0), controlPin(-1), pwmPin(-1) {}

void SSR::init(int controlPin, int pwmPin) {
    this->controlPin = controlPin;
    this->pwmPin = pwmPin;

    if (controlPin == -1 || pwmPin == -1) {
        Serial.println("SSR initialization failed: invalid pins.");
        return;
    }

    pinMode(controlPin, OUTPUT); // Устанавливаем пин включения/выключения как выход
    pinMode(pwmPin, OUTPUT);     // Устанавливаем пин ШИМ как выход

    digitalWrite(controlPin, LOW); // Начальное состояние: SSR выключен
    analogWrite(pwmPin, 0);        // Начальное состояние мощности: 0
    Serial.println("SSR initialized successfully.");
}

void SSR::setState(bool state) {
    enabled = state;
    digitalWrite(controlPin, enabled ? HIGH : LOW);
}

void SSR::setPWM(int level) {
    pwmLevel = constrain(map(level, 0, 100, 0, 255), 0, 255);
    analogWrite(pwmPin, pwmLevel);
}

bool SSR::isEnabled() const {
    return enabled;
}

int SSR::getPWMLevel() const {
    return pwmLevel;
}

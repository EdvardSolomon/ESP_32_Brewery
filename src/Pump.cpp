#include "Pump.h"

Pump::Pump() : enabled(false), pwmLevel(0), controlPin(-1), pwmPin(-1) {}

void Pump::init(int controlPin, int pwmPin) {
    this->controlPin = controlPin;
    this->pwmPin = pwmPin;

    if (controlPin == -1 || pwmPin == -1) {
        Serial.println("Pump initialization failed: invalid pins.");
        return;
    }

    Serial.print("Configuring Pump control pin: ");
    Serial.println(controlPin);
    pinMode(controlPin, OUTPUT); // Устанавливаем пин включения/выключения как выход
    Serial.print("Configuring Pump PWM pin: ");
    Serial.println(pwmPin);
    pinMode(pwmPin, OUTPUT);     // Устанавливаем пин ШИМ как выход

    digitalWrite(controlPin, LOW); // Начальное состояние: насос выключен
    analogWrite(pwmPin, 0);        // Начальное состояние мощности: 0
    Serial.println("Pump initialized successfully.");
}

void Pump::setState(bool state) {
    enabled = state;
    digitalWrite(controlPin, enabled ? HIGH : LOW);
}

void Pump::setPWM(int level) {
    pwmLevel = constrain(map(level, 0, 100, 0, 255), 0, 255);
    analogWrite(pwmPin, pwmLevel);
}

bool Pump::isEnabled() const {
    return enabled;
}

int Pump::getPWMLevel() const {
    return pwmLevel;
}

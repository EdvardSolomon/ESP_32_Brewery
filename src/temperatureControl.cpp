#include "temperatureControl.h"

// Конструктор
TemperatureControl::TemperatureControl(float target_temp, float hysteresis)
    : target_temp(target_temp), hysteresis(hysteresis), ssr_state(false) {}

// Обновление состояния SSR в зависимости от текущей температуры
void TemperatureControl::update(float current_temp) {
    if (current_temp <= (target_temp - hysteresis)) {
        ssr_state = true;  // Включить нагрев
    } else if (current_temp > target_temp) {
        ssr_state = false; // Выключить нагрев
    }

    setSSRState(ssr_state);
}

// Установка состояния SSR (реализация зависит от платформы)
void TemperatureControl::setSSRState(bool state) {
    ssr_state = state;
    if (state) {
        // Подать питание на SSR
        std::cout << "SSR включен (подан плюс)" << std::endl;
    } else {
        // Отключить SSR
        std::cout << "SSR выключен (снят плюс)" << std::endl;
    }
}

// Получение текущего состояния SSR
bool TemperatureControl::getSSRState() const {
    return ssr_state;
}

// Установка новой целевой температуры
void TemperatureControl::setTargetTemp(float temp) {
    target_temp = temp;
}

// Установка нового гистерезиса
void TemperatureControl::setHysteresis(float hysteresis) {
    this->hysteresis = hysteresis;
}

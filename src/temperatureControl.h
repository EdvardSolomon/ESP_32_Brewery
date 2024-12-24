#ifndef TEMPERATURE_CONTROL_H
#define TEMPERATURE_CONTROL_H

#include <iostream>

class TemperatureControl {
private:
    float target_temp;   // Целевая температура
    float hysteresis;    // Гистерезис
    bool ssr_state;      // Состояние SSR (вкл/выкл)

public:
    TemperatureControl(float target_temp, float hysteresis);

    void update(float current_temp);
    void setSSRState(bool state);
    bool getSSRState() const;

    void setTargetTemp(float temp);
    void setHysteresis(float hysteresis);
};

#endif // TEMPERATURE_CONTROL_H

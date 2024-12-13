#include "control.h"
#include <Arduino.h>


// GPIO for pump and SSR
#define PUMP_PIN 16
#define SSR_PIN 19

// Pump control variables
int pumpPWMLevel = 0;
bool pumpEnabled = false;

// SSR control variables
int ssrPWMLevel = 0;
bool ssrEnabled = false;

void initControl() {
    ledcSetup(0, 5000, 8);
    ledcAttachPin(PUMP_PIN, 0);
    updatePumpPWM();

    ledcSetup(1, 5000, 8);
    ledcAttachPin(SSR_PIN, 1);
    updateSSRPWM();
}

void updatePumpPWM() {
    int dutyCycle = pumpEnabled ? map(pumpPWMLevel, 0, 100, 0, 255) : 0;
    ledcWrite(0, dutyCycle);
}

void updateSSRPWM() {
    int dutyCycle = ssrEnabled ? map(ssrPWMLevel, 0, 100, 0, 255) : 0;
    ledcWrite(1, dutyCycle);
}

bool togglePump() {
    pumpEnabled = !pumpEnabled;
    updatePumpPWM();
    return pumpEnabled;
}

bool toggleSSR() {
    ssrEnabled = !ssrEnabled;
    updateSSRPWM();
    return ssrEnabled;
}

void setPumpPWMLevel(int level) {
    pumpPWMLevel = constrain(level, 0, 100);
    updatePumpPWM();
}

void setSSRPWMLevel(int level) {
    ssrPWMLevel = constrain(level, 0, 100);
    updateSSRPWM();
}

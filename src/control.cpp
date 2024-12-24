#include "control.h"

bool pumpEnabled = false;
int pumpPWMLevel = 0;

bool ssrEnabled = false;
int ssrPWMLevel = 0;

void initControl() {
    // Инициализация насосов и SSR
    pumpEnabled = false;
    pumpPWMLevel = 0;

    ssrEnabled = false;
    ssrPWMLevel = 0;
}

void togglePump() {
    pumpEnabled = !pumpEnabled;
}

void setPumpPWMLevel(int level) {
    if (level >= 0 && level <= 100) {
        pumpPWMLevel = level;
    }
}

void toggleSSR() {
    ssrEnabled = !ssrEnabled;
}

void setSSRPWMLevel(int level) {
    if (level >= 0 && level <= 100) {
        ssrPWMLevel = level;
    }
}

void appendControlInfoToJSON(JsonDocument& doc) {
    JsonObject control = doc.createNestedObject("control");
    control["pump_enabled"] = pumpEnabled;
    control["pump_pwm"] = pumpPWMLevel;
    control["ssr_enabled"] = ssrEnabled;
    control["ssr_pwm"] = ssrPWMLevel;
}

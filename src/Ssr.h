#ifndef SSR_H
#define SSR_H

#include <Arduino.h>

class SSR {
private:
    bool enabled;
    int pwmLevel;
    int controlPin;
    int pwmPin;

public:
    SSR();

    void init(int controlPin, int pwmPin);
    void setState(bool state);
    void setPWM(int level);
    bool isEnabled() const;
    int getPWMLevel() const;
};

#endif // SSR_H

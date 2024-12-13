#ifndef CONTROL_H
#define CONTROL_H

void initControl();
void updatePumpPWM();
void updateSSRPWM();
bool togglePump();
bool toggleSSR();
void setPumpPWMLevel(int level);
void setSSRPWMLevel(int level);

#endif // CONTROL_H

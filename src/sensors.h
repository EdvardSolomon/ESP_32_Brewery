#ifndef SENSORS_H
#define SENSORS_H

#include <DallasTemperature.h>

void initSensors();
float getTemperature(int sensorIndex);
String getSensorAddress(int sensorIndex);

#endif // SENSORS_H

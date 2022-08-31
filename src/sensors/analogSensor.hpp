#include <Arduino.h>

#ifndef ANALOGSENSOR_HPP
#define ANALOGSENSOR_HPP

class AnalogSensor
{
private:
    int pin;

public:
    AnalogSensor(uint8_t pin);
    ~AnalogSensor();
    uint16_t getValue();
    boolean isDetected(uint16_t limit);
};

#endif
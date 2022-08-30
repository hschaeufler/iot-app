#include <sensors/digitalSensor.hpp>
#include <Arduino.h>

DigitalSensor::DigitalSensor(int pin)
{
    pin = pin;
}

DigitalSensor::~DigitalSensor()
{
}

bool DigitalSensor::isDetected()
{
    int sensorVal = digitalRead(pin);
    bool isDetectedVal = sensorVal == 1 ? true : false;
    return isDetectedVal;
}

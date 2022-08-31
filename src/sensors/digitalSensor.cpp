#include <sensors/digitalSensor.hpp>
#include <Arduino.h>

DigitalSensor::DigitalSensor(int pin)
{
    this->pin = pin;
    pinMode(pin, INPUT);
}

DigitalSensor::~DigitalSensor()
{
}

bool DigitalSensor::isDetected()
{
    int sensorVal = digitalRead(this->pin);
    bool isDetectedVal = sensorVal == HIGH ? true : false;
    return isDetectedVal;
}

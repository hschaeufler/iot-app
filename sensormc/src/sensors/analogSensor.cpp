#include <sensors/analogSensor.hpp>
#include <Arduino.h>

AnalogSensor::AnalogSensor(uint8_t pin)
{
    this->pin = pin;
    pinMode(pin, INPUT);
}

AnalogSensor::~AnalogSensor()
{
}

uint16_t AnalogSensor::getValue()
{
    return analogRead(this->pin);
}

boolean AnalogSensor::isDetected(uint16_t limit)
{
    uint16_t value = analogRead(this->pin);
    return value >= limit ? true : false;
}
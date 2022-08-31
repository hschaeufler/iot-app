#include <sensors/waterSensor.hpp>
#include <sensors/analogSensor.hpp>
#include <Arduino.h>

bool WaterSensor::isWater(uint16_t limit)
{
    return AnalogSensor::isDetected(limit);
}

uint16_t WaterSensor::detectWater()
{
    return AnalogSensor::getValue();
}

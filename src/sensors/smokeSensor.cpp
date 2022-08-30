#include <sensors/smokeSensor.hpp>
#include <Arduino.h>

bool SmokeSensor::isSmokey()
{
    return DigitalSensor::isDetected();
}

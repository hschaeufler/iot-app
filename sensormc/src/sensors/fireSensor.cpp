#include "sensors/fireSensor.hpp"
#include <Arduino.h>

bool FireSensor::isFire()
{
    return DigitalSensor::isDetected();
}

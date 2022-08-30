#include <sensors/digitalSensor.hpp>

#ifndef SMOKESENSOR_HPP
#define SMOKESENSOR_HPP

class SmokeSensor : DigitalSensor
{
private:
public:
    using DigitalSensor::DigitalSensor;
    bool isSmokey();
};

#endif
#include <sensors/digitalSensor.hpp>

#ifndef FIRESENSOR_HPP
#define FIRESENSOR_HPP

class FireSensor : DigitalSensor
{
private:
public:
    using DigitalSensor::DigitalSensor;
    bool isFire();
};

#endif
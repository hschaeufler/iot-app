#include <sensors/analogSensor.hpp>

#ifndef WATERSENSOR_HPP
#define WATERSENSOR_HPP

class WaterSensor : AnalogSensor
{
private:
public:
    using AnalogSensor::AnalogSensor;
    bool isWater(uint16_t limit);
    uint16_t detectWater();
};

#endif
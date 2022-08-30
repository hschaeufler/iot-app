#ifndef DIGITALSENSOR_HPP
#define DIGITALSENSOR_HPP

class DigitalSensor
{
private:
    int pin;

public:
    DigitalSensor(int pin);
    ~DigitalSensor();
    bool isDetected();
};

#endif
#ifndef TRAFFICLIGHT_HPP
#define TRAFFICLIGHT_HPP

class TrafficLight
{
private:
    int redPin;
    int yellowPin;
    int greenPin;

public:
    TrafficLight(int redPin, int yellowPin, int greenPin);
    ~TrafficLight();
    void setGreen();
    void setYellow();
    void setRed();
    void shuttOff();
};

#endif
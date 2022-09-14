#include <actuators/TrafficLight.hpp>
#include <Arduino.h>

TrafficLight::TrafficLight(int redPin, int yellowPin, int greenPin)
{
    this->redPin = redPin;
    this->yellowPin = yellowPin;
    this->greenPin = greenPin;
    pinMode(this->redPin, OUTPUT);
    pinMode(this->yellowPin, OUTPUT);
    pinMode(this->greenPin, OUTPUT);
}

TrafficLight::~TrafficLight()
{
}

void TrafficLight::setGreen()
{
    digitalWrite(this->greenPin, HIGH);
    digitalWrite(this->yellowPin, LOW);
    digitalWrite(this->redPin, LOW);
}
void TrafficLight::setYellow()
{
    digitalWrite(this->greenPin, LOW);
    digitalWrite(this->yellowPin, HIGH);
    digitalWrite(this->redPin, LOW);
}
void TrafficLight::setRed()
{
    digitalWrite(this->greenPin, LOW);
    digitalWrite(this->yellowPin, LOW);
    digitalWrite(this->redPin, HIGH);
}
void TrafficLight::shuttOff()
{
    digitalWrite(this->greenPin, LOW);
    digitalWrite(this->yellowPin, LOW);
    digitalWrite(this->redPin, LOW);
}
#include <actuators/Buzzer.hpp>
#include <Arduino.h>

Buzzer::Buzzer(int buzzerPin)
{
    this->buzzerPin = buzzerPin;
    pinMode(this->buzzerPin, OUTPUT);
}

Buzzer::~Buzzer()
{
}

void Buzzer::startBeep()
{
    digitalWrite(this->buzzerPin, HIGH);
}
void Buzzer::stopBeep()
{
    digitalWrite(this->buzzerPin, LOW);
}
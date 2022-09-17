#ifndef BUZZER_HPP
#define BUZZER_HPP

class Buzzer
{
private:
    int buzzerPin;

public:
    Buzzer(int buzzerPin);
    ~Buzzer();
    void startBeep();
    void stopBeep();
};

#endif
#pragma once
#include <Arduino.h>
#include <Servo.h>

class ServoMotor {
private:
    uint8_t pin;
    Servo servo;

public:
    ServoMotor(uint8_t pin);
    void begin();
    void setAngle(uint8_t angle);
    uint8_t getAngle();
};
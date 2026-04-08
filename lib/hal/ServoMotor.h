#pragma once
#include <Arduino.h>
#include <Servo.h>

class ServoMotor {
private:
    uint8_t pin;
    Servo servo;

    uint8_t startAngle;
    uint8_t targetAngle;
    uint32_t moveStartTime;
    uint32_t duration;
    bool isMoving;
    uint8_t currentAngle;

public:
    ServoMotor(uint8_t pin);
    void begin();
    void update(unsigned long currentMillis);

    void moveTo(uint8_t angle, uint32_t moveDuration, uint32_t startTime);
    void setAngleInstantly(uint8_t angle);

    bool getIsMoving() const;
    uint8_t getAngle();
};
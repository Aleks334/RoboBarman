#include "ServoMotor.h"

ServoMotor::ServoMotor(uint8_t pin) : pin(pin) {}

void ServoMotor::begin() {
    servo.attach(pin);
}

void ServoMotor::setAngle(uint8_t angle) {
    if (angle > 180) 
    {
        angle = 180; 
    }
    
    servo.write(angle);
}

uint8_t ServoMotor::getAngle() {
    return servo.read();
}
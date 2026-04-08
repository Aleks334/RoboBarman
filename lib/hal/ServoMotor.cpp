#include "ServoMotor.h"
#include <Arduino.h>

ServoMotor::ServoMotor(uint8_t pin) 
    : pin(pin), startAngle(0), targetAngle(0), 
      moveStartTime(0), duration(0), isMoving(false), currentAngle(0) {}

void ServoMotor::begin() {
    servo.attach(pin);
}

void ServoMotor::update(unsigned long currentMillis) {
    if(!isMoving) return;

    unsigned long elapsedTime = currentMillis - moveStartTime;

    if(elapsedTime >= duration) {
        currentAngle = targetAngle;
        servo.write(targetAngle);
        isMoving = false;
    } else {
        float normalizedProgress = (float)elapsedTime / (float)duration;
        int16_t anglesDiffrence = (int16_t)targetAngle - (int16_t)startAngle;
        
        uint8_t currentLerpedPosition = startAngle + (uint8_t)(anglesDiffrence * normalizedProgress);
        servo.write(currentLerpedPosition);
    }
}

void ServoMotor::setAngleInstantly(uint8_t angle) {
    if (angle > 180) {
        angle = 180; 
    }

    targetAngle = angle;
    startAngle = angle;
    currentAngle = angle;
    isMoving = false;
    servo.write(angle);
}

void ServoMotor::moveTo(uint8_t angle, uint32_t moveDuration, uint32_t startTime) {
    if (angle > 180) {
        angle = 180; 
    }

    startAngle = currentAngle;
    targetAngle = angle;
    duration = moveDuration;
    moveStartTime = startTime;
    isMoving = true;
}

uint8_t ServoMotor::getAngle() {
    return currentAngle;
}

bool ServoMotor::getIsMoving() const {
    return isMoving;
}

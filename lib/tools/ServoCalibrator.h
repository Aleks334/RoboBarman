#pragma once

#include <Arduino.h>
#include "Sensor.h"
#include "ServoMotor.h"
#include "RgbLed.h"

class ServoCalibrator {
private:
    ServoMotor* servo;
    Sensor* sensor;
    RgbLed* led;

    const uint8_t MIN_ANGLE = 30;
    const uint8_t MAX_ANGLE = 150;
    int currentAngle = MIN_ANGLE;
    bool locked = false;
    unsigned long lastStep = 0;

    void blinkResult(int angle);

public:
    ServoCalibrator(
        uint8_t servoPin, 
        uint8_t trigPin,
        uint8_t echoPin, 
        uint8_t rPin,
        uint8_t gPin,
        uint8_t bPin,
        uint16_t sensorDebounceMs, 
        uint16_t sensorThresholdCm, 
        bool ledCommonAnode
    );
    ~ServoCalibrator();

    void begin();
    void update();
};
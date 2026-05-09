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

    int currentAngle = 20;
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
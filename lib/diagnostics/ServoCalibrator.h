#pragma once
#include <Arduino.h>
#include "GlobalConfig.h"

#include "Sensor.h"
#include "ServoMotor.h"
#include "RgbLed.h"

class ServoCalibrator {
private:
    Sensor* sensor;
    ServoMotor* servo;
    RgbLed* led;

    int currentAngle = 90;
    bool locked = false;

    void blinkResult(int angle);

public:
    ServoCalibrator();
    ~ServoCalibrator();
    void update();
};
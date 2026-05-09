#pragma once
#include <Arduino.h>

struct StationPins {
    uint8_t sensorTrigPin;
    uint8_t sensorEchoPin;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};
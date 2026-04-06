#pragma once
#include <Arduino.h>

class Pump {
private:
    uint8_t pin;

public:
    Pump(uint8_t pin);
    void begin();
    void start();
    void stop();
    bool isRunning() const;
};
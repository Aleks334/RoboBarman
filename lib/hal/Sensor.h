#pragma once
#include <Arduino.h>

class Sensor {
private:
    uint8_t pin;

public:
    Sensor(uint8_t pin);
    void begin();
    
    bool isActive() const; 
};
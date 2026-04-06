#pragma once
#include <Arduino.h>

class Sensor {
private:
    uint8_t pin;
    bool isSimulationMode;
    bool simulatedState;

public:
    Sensor(uint8_t pin);
    void begin();
    
    bool isActive() const; 

    void simulateState(bool state);
    void disableSimulation();
};
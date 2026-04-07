#pragma once
#include <Arduino.h>

class Sensor {
private:
    uint8_t pin;
    uint16_t debounceDelay;

    bool lastRawState;
    bool debouncedState;
    unsigned long lastDebounceTime;

    bool isSimulationMode;
    bool simulatedState;

    bool readSensorValue();

public:
    Sensor(uint8_t pin, uint16_t debounceDelayMs = 50);
    void begin();
    
    void update(unsigned long currentMillis);
    bool hasDetectedObject() const; 

    void simulateState(bool state);
    void disableSimulation();
};
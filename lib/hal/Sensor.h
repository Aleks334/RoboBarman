#pragma once
#include <Arduino.h>

class Sensor {
private:
    static constexpr uint16_t MIN_PING_INTERVAL_MS = 60;   
    static constexpr uint16_t TRIGGER_PULSE_US = 10;
    static constexpr unsigned long PING_TIMEOUT_US = 3000;
    
    uint8_t trigPin;
    uint8_t echoPin;
    uint16_t debounceDelay;
    uint8_t detectionThresholdCm;

    bool lastRawState;
    bool debouncedState;
    unsigned long lastDebounceTime;
    unsigned long lastPingTime;

    bool isSimulationMode;
    bool simulatedState;

    bool readSensorValue();
    double calculateDistance(unsigned long echoDurationUs) const;

public:
    Sensor(uint8_t trigPin, uint8_t echoPin, uint16_t debounceDelayMs = 50, uint8_t thresholdCm = 8);
    void begin();
    
    void update(unsigned long currentMillis);
    bool hasDetectedObject() const; 

    void simulateState(bool state);
    void disableSimulation();
};

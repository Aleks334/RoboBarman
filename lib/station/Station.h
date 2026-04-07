#pragma once
#include "Sensor.h"
#include "RgbLed.h"
#include "Queue.h"
#include "Barman.h"

enum class StationState : uint8_t {
    IDLE,
    OCCUPIED,
    IN_PROGRESS,
    READY
};

class Station {
private:
    uint8_t id;
    Sensor& sensor;
    RgbLed& led;
    Queue& queue;
    Barman& barman;
    
    StationState state;
    
    unsigned long lastBlinkTime;
    uint16_t blinkInterval;
    bool ledToggleState;

    void updateLed(unsigned long currentMillis);

public:
    Station(uint8_t id, Sensor& s, RgbLed& l, Queue& q, Barman& b, uint16_t blinkInterval);
    
    void begin();
    void update(unsigned long currentMillis);
    
    StationState getState() const;
};
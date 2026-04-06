#pragma once
#include <stdint.h>
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
    
    unsigned long lastFlashTime;
    static constexpr uint16_t FLASH_INTERVAL = 300;

    void updateLed(unsigned long currentMillis);

public:
    Station(uint8_t id, Sensor& s, RgbLed& l, Queue& q, Barman& b);
    
    void begin();
    void update(unsigned long currentMillis);
    
    StationState getState() const;
};
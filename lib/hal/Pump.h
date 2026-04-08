#pragma once
#include <Arduino.h>

enum class PumpState : uint8_t {
    IDLE,
    START_DELAY,
    RUNNING,
    STOP_DELAY
};

class Pump {
private:
    uint8_t pin;
    uint32_t startDelayDuration;
    uint32_t stopDelayDuration;
    uint32_t actionStartTime;
    PumpState currentState;
    unsigned long currentTime;
    uint32_t requestedPourDuration;

public:
    Pump(uint8_t pin, uint32_t startDelayDuration, uint32_t stopDelayDuration);
    void begin();
    void pour(uint32_t duration, uint32_t startTime);
    void update(unsigned long currentMillis);

    // void start();
    // void stop();
    void stopInstantly();
    
    PumpState getState() const;
    bool isBusy() const;
    bool isRunning() const;
};
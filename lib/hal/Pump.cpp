#include "Pump.h"
#include <Arduino.h>

Pump::Pump(uint8_t pin, uint32_t startDelayDuration, uint32_t stopDelayDuration) 
  : pin(pin),
    startDelayDuration(startDelayDuration),
    stopDelayDuration(stopDelayDuration),
    actionStartTime(0),
    currentState(PumpState::IDLE),
    currentTime(0),
    requestedPourDuration(0) {}

void Pump::begin() {
    pinMode(pin, OUTPUT);
    stopInstantly();
}

void Pump::pour(uint32_t duration, uint32_t startTime) {
    if (currentState == PumpState::IDLE) {
        requestedPourDuration = duration;
        actionStartTime = startTime;
        currentState = PumpState::START_DELAY;
    }
}

void Pump::update(unsigned long currentMillis) {
    currentTime = currentMillis;
    
    switch (currentState) {
        case PumpState::START_DELAY:
            if (currentMillis - actionStartTime >= startDelayDuration) {
                digitalWrite(pin, HIGH);
                actionStartTime = currentMillis;
                currentState = PumpState::RUNNING;
            }
            break;

        case PumpState::RUNNING:
            if (currentMillis - actionStartTime >= requestedPourDuration) {
                digitalWrite(pin, LOW);
                actionStartTime = currentMillis;
                currentState = PumpState::STOP_DELAY;
            }
            break;

        case PumpState::STOP_DELAY:
            if (currentMillis - actionStartTime >= stopDelayDuration) {
                currentState = PumpState::IDLE;
            }
            break;

        default:
            break;
    }
}

PumpState Pump::getState() const {
    return currentState;
}

bool Pump::isBusy() const {
    return currentState != PumpState::IDLE;
}

bool Pump::isRunning() const {
    return currentState == PumpState::RUNNING;
}

void Pump::stopInstantly() {
    digitalWrite(pin, LOW);
    currentState = PumpState::IDLE;
    actionStartTime = 0;
}

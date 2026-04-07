#include "Sensor.h"

Sensor::Sensor(uint8_t pin, uint16_t debounceDelayMs) 
    : pin(pin), debounceDelay(debounceDelayMs), 
      lastRawState(false), debouncedState(false), lastDebounceTime(0),
      isSimulationMode(false), simulatedState(false) {}

void Sensor::begin() {
    pinMode(pin, INPUT);
    lastRawState = readSensorValue();
    debouncedState = lastRawState;
}

void Sensor::update(unsigned long currentMillis) {
    if (isSimulationMode) return;

    bool currentState = readSensorValue();

    if (currentState != lastRawState) {
        lastDebounceTime = currentMillis;
    }

    if ((currentMillis - lastDebounceTime) > debounceDelay) {
        debouncedState = currentState;
    }

    lastRawState = currentState;
}

bool Sensor::hasDetectedObject() const {
    return isSimulationMode ? simulatedState : debouncedState;
}

void Sensor::simulateState(bool state) {
    isSimulationMode = true;
    simulatedState = state;
}

void Sensor::disableSimulation() {
    isSimulationMode = false;
}

bool Sensor::readSensorValue() {
    return digitalRead(pin) == LOW;
}

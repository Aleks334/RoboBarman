#include "Sensor.h"

Sensor::Sensor(uint8_t pin) 
    : pin(pin), isSimulationMode(false), simulatedState(false) {}

void Sensor::begin() {
    pinMode(pin, INPUT_PULLUP);
}

bool Sensor::isActive() const {
    if (isSimulationMode) {
        return simulatedState;
    }
    return digitalRead(pin) == LOW; 
}

void Sensor::simulateState(bool state) {
    isSimulationMode = true;
    simulatedState = state;
}

void Sensor::disableSimulation() {
    isSimulationMode = false;
}

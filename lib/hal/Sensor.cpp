#include "Sensor.h"

Sensor::Sensor(uint8_t pin) : pin(pin) {}

void Sensor::begin() {
    pinMode(pin, INPUT_PULLUP);
}

bool Sensor::isActive() const {
    return digitalRead(pin) == LOW; 
}
#include "Pump.h"

Pump::Pump(uint8_t pin) : pin(pin) {}

void Pump::begin() {
    pinMode(pin, OUTPUT);
    stop();
}

void Pump::start() {
    digitalWrite(pin, HIGH);
}

void Pump::stop() {
    digitalWrite(pin, LOW);
}

bool Pump::isRunning() const {
    return digitalRead(pin) == HIGH;
}
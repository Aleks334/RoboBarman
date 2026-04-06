#include "Led.h"

Led::Led(uint8_t pin) : pin(pin) {}

void Led::begin() {
    pinMode(pin, OUTPUT);
    off();
}

void Led::on() {
    digitalWrite(pin, HIGH);
}

void Led::off() {
    digitalWrite(pin, LOW);
}

void Led::toggle() {
    digitalWrite(pin, !isOn());
}

bool Led::isOn() const {
    return digitalRead(pin) == HIGH;
}
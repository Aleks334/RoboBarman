#include "RgbLed.h"

RgbLed::RgbLed(uint8_t r, uint8_t g, uint8_t b, bool commonAnode) 
    : pinR(r), pinG(g), pinB(b), commonAnode(commonAnode), currentColor(Color::OFF) {}

void RgbLed::begin() {
    pinMode(pinR, OUTPUT);
    pinMode(pinG, OUTPUT);
    pinMode(pinB, OUTPUT);
    off();
}

void RgbLed::setColor(Color color) {
    currentColor = color;

    bool r = false, g = false, b = false;

    switch (color) {
        case Color::RED:    
            r = true;
            break;
        case Color::GREEN:  
            g = true; 
            break;
        case Color::BLUE:   
            b = true; 
            break;
        case Color::YELLOW: 
            r = g = true; 
            break;
        case Color::WHITE:  
            r = g = b = true; 
            break;
        case Color::OFF:    
            break;
    }

    digitalWrite(pinR, commonAnode ? !r : r);
    digitalWrite(pinG, commonAnode ? !g : g);
    digitalWrite(pinB, commonAnode ? !b : b);
}

void RgbLed::off() {
    setColor(Color::OFF);
}

Color RgbLed::getColor() const {
    return currentColor;
}
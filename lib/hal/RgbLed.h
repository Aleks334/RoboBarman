#pragma once
#include <Arduino.h>

enum class Color : uint8_t {
    OFF,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    WHITE
};

class RgbLed {
private:
    uint8_t pinR, pinG, pinB;
    bool commonAnode; // if true then LED is active low
    Color currentColor;

public:
    RgbLed(uint8_t r, uint8_t g, uint8_t b, bool commonAnode = false);
    
    void begin();
    void setColor(Color color);
    void off();

    Color getColor() const; 
};
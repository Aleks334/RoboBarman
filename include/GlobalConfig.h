#pragma once
#include <Arduino.h>

namespace GlobalConfig {

    static constexpr uint8_t NUM_STATIONS = 3;
    static constexpr uint8_t NO_STATION = 255;

    static constexpr uint8_t PIN_PUMP = 8;
    static constexpr uint8_t PIN_SERVO = 9;

    static constexpr uint8_t PINS_SENSOR[NUM_STATIONS] = {2, 3, 4}; 
    
    static constexpr uint8_t PINS_RGB_R[NUM_STATIONS] = {A0, A3, 10}; 
    static constexpr uint8_t PINS_RGB_G[NUM_STATIONS] = {A1, A4, 11};
    static constexpr uint8_t PINS_RGB_B[NUM_STATIONS] = {A2, A5, 12};

    static constexpr bool LED_COMMON_ANODE = false; 

    static constexpr uint32_t MOVE_DURATION = 2000; 
    static constexpr uint32_t FILL_DURATION = 3000;
    static constexpr uint16_t LED_FLASH_INTERVAL = 300; 

    static constexpr uint8_t SERVO_POS_IDLE = 0;   
    static constexpr uint8_t SERVO_POS_STATIONS[NUM_STATIONS] = {30, 90, 150};

    static constexpr uint16_t SENSOR_DEBOUNCE_MS = 50;   
}

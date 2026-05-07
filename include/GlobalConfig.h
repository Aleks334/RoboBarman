#pragma once
#include <Arduino.h>

namespace GlobalConfig {

    static constexpr uint8_t QUEUE_CAPACITY = 10;
    static constexpr uint8_t NUM_STATIONS = 3;

    static constexpr uint8_t PIN_PUMP = 3;
    static constexpr uint8_t PIN_SERVO = 10;

    struct StationPins {
        uint8_t sensorTrigPin;
        uint8_t sensorEchoPin;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };
 
    static constexpr StationPins STATIONS_CONFIG[NUM_STATIONS] = {
        { 6, 5, 11, 13, A0 },
        { 7, 8, A1, A2, A3 },
        { 9, 12, A4, A5, 4 }
    };

    static constexpr bool LED_COMMON_ANODE = true; 

    /* NOTE: This parameter impacts the speed of servomotor */
    static constexpr uint32_t MOVE_DURATION_MS = 1500; 

    static constexpr uint32_t FILL_DURATION_MS = 4000;
    static constexpr uint32_t PUMP_START_DELAY = 500;
    static constexpr uint32_t PUMP_STOP_DELAY = 500;

    static constexpr uint16_t LED_BLINK_INTERVAL_MS = 350; 

    static constexpr uint8_t SERVO_POS_IDLE = 90;   
    static constexpr uint8_t SERVO_STATIONS_DEGREE_ANGLES[NUM_STATIONS] = {60, 120, 150};

    static constexpr uint16_t SENSOR_DEBOUNCE_MS = 50;  
    static constexpr uint16_t SENSOR_DETECTION_TRESHOLD_CM = 8; 
}

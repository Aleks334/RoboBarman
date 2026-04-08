#pragma once
#include <Arduino.h>

namespace GlobalConfig {

    static constexpr uint8_t QUEUE_CAPACITY = 5;
    static constexpr uint8_t NUM_STATIONS = 1;

    static constexpr uint8_t PIN_PUMP = 8;
    static constexpr uint8_t PIN_SERVO = 9;

    struct StationPins {
        uint8_t sensor;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    static constexpr StationPins STATIONS_CONFIG[NUM_STATIONS] = {
        {2, A0, A1, A2}
    };

    static constexpr bool LED_COMMON_ANODE = true; 

    /* NOTE: This parameter impacts the speed of servomotor */
    static constexpr uint32_t MOVE_DURATION_MS = 500; 

    static constexpr uint32_t FILL_DURATION_MS = 2000;
    static constexpr uint32_t PUMP_START_DELAY = 2000;
    static constexpr uint32_t PUMP_STOP_DELAY = 3000;

    static constexpr uint16_t LED_BLINK_INTERVAL_MS = 250; 

    static constexpr uint8_t SERVO_POS_IDLE = 90;   
    static constexpr uint8_t SERVO_STATIONS_DEGREE_ANGLES[NUM_STATIONS] = {155};

    static constexpr uint16_t SENSOR_DEBOUNCE_MS = 500;   
}

#pragma once

#include <Arduino.h>
#include "StationPins.h"
#include "Pump.h"
#include "Sensor.h"
#include "ServoMotor.h"
#include "RgbLed.h"

class HardwareTester {
private:
    Pump* pump;
    ServoMotor* servo;
    
    Sensor** stationSensors;
    RgbLed** stationLeds;

    uint8_t numStations;
    uint32_t pumpStartDelay;
    uint32_t pumpStopDelay;
    uint32_t moveDurationMs;
    uint32_t fillDurationMs;

public:
    HardwareTester(
        uint8_t numStations, 
        const StationPins* stationConfigs, 
        uint8_t pinPump, 
        uint32_t pumpStartDelay, 
        uint32_t pumpStopDelay, 
        uint8_t pinServo, 
        bool ledCommonAnode, 
        uint16_t sensorDebounceMs, 
        uint16_t sensorThresholdCm,
        uint32_t moveDurationMs,
        uint32_t fillDurationMs
    );
    ~HardwareTester();

    void begin();
    void tick(unsigned long durationMs);
    void runDiagnostics();
};
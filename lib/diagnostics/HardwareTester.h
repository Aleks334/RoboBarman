#pragma once
#include <Arduino.h>
#include "GlobalConfig.h"

#include "Sensor.h"
#include "Pump.h"  
#include "ServoMotor.h"  
#include "RgbLed.h"

class HardwareTester {
private:
    ServoMotor* servo;
    Pump* pump;
    Sensor* stationSensors[GlobalConfig::NUM_STATIONS];
    RgbLed* stationLeds[GlobalConfig::NUM_STATIONS];

public:
    HardwareTester();
    ~HardwareTester();
    
    void begin();
    void tick(unsigned long durationMs);
    void runFullDiagnostics(); 
};
#pragma once

#include "GlobalConfig.h"
#include "Queue.h"
#include "Pump.h"
#include "ServoMotor.h"
#include "Sensor.h"
#include "RgbLed.h"
#include "Barman.h"
#include "Station.h"

using namespace GlobalConfig;

class RoboBarmanController {
    private:
        Queue ordersQueue;
        Pump barmanPump;
        ServoMotor barmanServo;
        Barman barman;
        Station* stations[NUM_STATIONS];
    
    public:
     void init();
     void update();
};
#pragma once
#include "Led.h"
#include "RgbLed.h"
#include "Pump.h"
#include "Sensor.h"
#include "ServoMotor.h"
#include "../test_common.h"

extern Led* testLed;
extern RgbLed* testRgbLed;
extern Pump* testPump;
extern Sensor* testSensor;
extern ServoMotor* testServo;

using namespace TestUtils;

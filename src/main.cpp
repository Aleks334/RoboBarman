#include <Arduino.h>
#include "GlobalConfig.h"
// #include "RoboBarmanController.h"
#include "HardwareTester.h"
//#include "ServoCalibrator.h"

using namespace GlobalConfig;

//RoboBarmanController controller;

HardwareTester tester(
    NUM_STATIONS,
    STATIONS_CONFIG,
    PIN_PUMP,
    PUMP_START_DELAY,
    PUMP_STOP_DELAY,
    PIN_SERVO,
    LED_COMMON_ANODE,
    SENSOR_DEBOUNCE_MS,
    SENSOR_DETECTION_TRESHOLD_CM,
    MOVE_DURATION_MS,
    FILL_DURATION_MS
);

// ServoCalibrator calibrator(
//     PIN_SERVO,
//     STATIONS_CONFIG->sensorTrigPin,
//     STATIONS_CONFIG->sensorEchoPin,
//     STATIONS_CONFIG->red,
//     STATIONS_CONFIG->green,
//     STATIONS_CONFIG->blue,
//     SENSOR_DEBOUNCE_MS,
//     SENSOR_DETECTION_TRESHOLD_CM,
//     LED_COMMON_ANODE
// );

void setup() {
    tester.begin();
}

void loop() {
    while(true) {
        tester.runDiagnostics();
    }
}
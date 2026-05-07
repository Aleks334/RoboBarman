#include <Arduino.h>
#include "HardwareTester.h"
#include "ServoCalibrator.h"

HardwareTester tester;
ServoCalibrator calibrator;

void setup() {
    tester.begin();
}

void loop() {
    tester.runFullDiagnostics();
    //calibrator.update();
}

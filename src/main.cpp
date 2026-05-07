#include <Arduino.h>
#include "RoboBarmanController.h"
// #include "avr8-stub.h"
// #include "app_api.h"

RoboBarmanController controller;

void setup() {
    //debug_init();
    controller.init();
}

void loop() {
    controller.update();
}

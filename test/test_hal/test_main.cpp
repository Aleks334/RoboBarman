#include "unity.h"
#include <Arduino.h>

#include "test_hal_globals.h"
#include "led/test_led.h"
#include "rgb_led/test_rgb_led.h"
#include "pump/test_pump.h"
#include "sensor/test_sensor.h"
#include "servomotor/test_servomotor.h"

const uint8_t TEST_LED_PIN = 13;
const uint8_t TEST_R_LED_PIN = 1;
const uint8_t TEST_G_LED_PIN = 2;
const uint8_t TEST_B_LED_PIN = 3;
const uint8_t TEST_PUMP_PIN = 8;
const uint8_t TEST_SENSOR_TRIG_PIN = 9;
const uint8_t TEST_SENSOR_ECHO_PIN = 10;
const uint8_t TEST_SERVO_PIN = 11;

const uint32_t TEST_PUMP_START_DELAY_MS = 100;
const uint32_t TEST_PUMP_STOP_DELAY_MS = 100;

Led* testLed;
RgbLed* testRgbLed;
Pump* testPump;
Sensor* testSensor;
ServoMotor* testServo;

void setUp() {
    clock = 0;
    testLed = new Led(TEST_LED_PIN);
    testRgbLed = new RgbLed(TEST_R_LED_PIN, TEST_G_LED_PIN, TEST_B_LED_PIN);
    testPump = new Pump(TEST_PUMP_PIN, TEST_PUMP_START_DELAY_MS, TEST_PUMP_STOP_DELAY_MS);
    testSensor = new Sensor(TEST_SENSOR_TRIG_PIN, TEST_SENSOR_ECHO_PIN);
    testServo = new ServoMotor(TEST_SERVO_PIN);
    
    testLed->begin();
    testRgbLed->begin();
    testPump->begin();
    testSensor->begin();
    testServo->begin();
}

void tearDown() {
    delete testLed;
    delete testRgbLed;
    delete testPump;
    delete testSensor;
    delete testServo;
}

void setup() {
    delay(2000);
    UNITY_BEGIN();

    run_led_tests();
    run_rgb_led_tests();
    run_pump_tests();
    run_sensor_tests();
    run_servo_tests();

    UNITY_END();
}

void loop() {}

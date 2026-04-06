#include "unity.h"
#include <Arduino.h>
#include "Led.h"
#include "Pump.h"
#include "Sensor.h"
#include "ServoMotor.h"

const uint8_t TEST_LED_PIN = 13;
const uint8_t TEST_PUMP_PIN = 8;
const uint8_t TEST_SENSOR_PIN = 9;
const uint8_t TEST_SERVO_PIN = 10;

Led testLed(TEST_LED_PIN);
Pump testPump(TEST_PUMP_PIN);
Sensor testSensor(TEST_SENSOR_PIN);
ServoMotor testServo(TEST_SERVO_PIN);

void setUp() {
    testLed.begin();
    testPump.begin();
    testSensor.begin();
    testServo.begin();
}

void tearDown() {}

void test_should_turn_on_led() {
    testLed.on();
    TEST_ASSERT_TRUE(testLed.isOn());
}

void test_should_turn_off_led() {
    testLed.on();
    testLed.off();
    TEST_ASSERT_FALSE(testLed.isOn());
}

void test_should_toggle_led_state() {
    testLed.off();
    testLed.toggle();
    TEST_ASSERT_TRUE(testLed.isOn());
    
    testLed.toggle();
    TEST_ASSERT_FALSE(testLed.isOn());
}

void test_should_start_pump() {
    testPump.start();
    TEST_ASSERT_TRUE(testPump.isRunning());
}

void test_should_stop_pump() {
    testPump.start();
    testPump.stop();
    TEST_ASSERT_FALSE(testPump.isRunning());
}

void test_should_default_to_inactive_when_unpressed() {
    TEST_ASSERT_FALSE(testSensor.isActive());
}

void test_should_set_and_read_servo_angle() {
    testServo.setAngle(90);
    TEST_ASSERT_EQUAL(90, testServo.getAngle());
}

void test_should_clamp_servo_angle_to_180() {
    testServo.setAngle(200);
    TEST_ASSERT_EQUAL(180, testServo.getAngle());
}

void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_should_turn_on_led);
    RUN_TEST(test_should_turn_off_led);
    RUN_TEST(test_should_toggle_led_state);

    RUN_TEST(test_should_start_pump);
    RUN_TEST(test_should_stop_pump);

    RUN_TEST(test_should_default_to_inactive_when_unpressed);

    RUN_TEST(test_should_set_and_read_servo_angle);
    RUN_TEST(test_should_clamp_servo_angle_to_180);

    UNITY_END();
}

void loop() {}

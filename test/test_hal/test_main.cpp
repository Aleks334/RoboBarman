#include "unity.h"
#include <Arduino.h>
#include "Led.h"
#include "RgbLed.h"
#include "Pump.h"
#include "Sensor.h"
#include "ServoMotor.h"

const uint8_t TEST_LED_PIN = 13;
const uint8_t TEST_R_LED_PIN = 1;
const uint8_t TEST_G_LED_PIN = 2;
const uint8_t TEST_B_LED_PIN = 3;

const uint8_t TEST_PUMP_PIN = 8;
const uint8_t TEST_SENSOR_PIN = 9;
const uint8_t TEST_SERVO_PIN = 10;

Led* testLed;
RgbLed* testRgbLed;

Pump* testPump;
Sensor* testSensor;
ServoMotor* testServo;

void setUp() {
    testLed = new Led(TEST_LED_PIN);
    testRgbLed = new RgbLed(TEST_R_LED_PIN, TEST_G_LED_PIN, TEST_B_LED_PIN);

    testPump = new Pump(TEST_PUMP_PIN);
    testSensor = new Sensor(TEST_SENSOR_PIN);
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

void test_should_turn_on_led() {
    testLed->on();
    TEST_ASSERT_TRUE(testLed->isOn());
}

void test_should_turn_off_led() {
    testLed->on();
    testLed->off();
    TEST_ASSERT_FALSE(testLed->isOn());
}

void test_should_toggle_led_state() {
    testLed->off();
    testLed->toggle();
    TEST_ASSERT_TRUE(testLed->isOn());
    
    testLed->toggle();
    TEST_ASSERT_FALSE(testLed->isOn());
}

void test_should_start_pump() {
    testPump->start();
    TEST_ASSERT_TRUE(testPump->isRunning());
}

void test_should_stop_pump() {
    testPump->start();
    testPump->stop();
    TEST_ASSERT_FALSE(testPump->isRunning());
}

void test_sensor_should_return_simulated_value() {
    testSensor->simulateState(true);
    TEST_ASSERT_TRUE(testSensor->hasDetectedObject());
    
    testSensor->simulateState(false);
    TEST_ASSERT_FALSE(testSensor->hasDetectedObject());
}

void test_should_set_and_read_servo_angle() {
    testServo->setAngle(90);
    TEST_ASSERT_EQUAL(90, testServo->getAngle());
}

void test_should_clamp_servo_angle_to_180() {
    testServo->setAngle(200);
    TEST_ASSERT_EQUAL(180, testServo->getAngle());
}

void test_should_rgb_led_be_off_after_init() {
    TEST_ASSERT_EQUAL(Color::OFF, testRgbLed->getColor());
}

void test_should_change_and_store_color() {
    testRgbLed->setColor(Color::GREEN);
    TEST_ASSERT_EQUAL(Color::GREEN, testRgbLed->getColor());
    
    testRgbLed->setColor(Color::RED);
    TEST_ASSERT_EQUAL(Color::RED, testRgbLed->getColor());
}

void test_should_turn_off() {
    testRgbLed->setColor(Color::RED);
    testRgbLed->off();
    TEST_ASSERT_EQUAL(Color::OFF, testRgbLed->getColor());
}

void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_should_turn_on_led);
    RUN_TEST(test_should_turn_off_led);
    RUN_TEST(test_should_toggle_led_state);

    RUN_TEST(test_should_start_pump);
    RUN_TEST(test_should_stop_pump);

    RUN_TEST(test_should_set_and_read_servo_angle);
    RUN_TEST(test_should_clamp_servo_angle_to_180);

    RUN_TEST(test_should_rgb_led_be_off_after_init);
    RUN_TEST(test_should_change_and_store_color);
    RUN_TEST(test_should_turn_off);

    UNITY_END();
}

void loop() {}

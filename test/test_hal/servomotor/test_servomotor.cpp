#include "unity.h"
#include <Arduino.h>
#include "../test_hal_globals.h"
#include "test_servomotor.h"

void test_should_set_and_read_servo_angle() {
    testServo->setAngleInstantly(90);
    TEST_ASSERT_EQUAL(90, testServo->getAngle());
}

void test_should_clamp_servo_angle_instantly_to_180() {
    testServo->setAngleInstantly(200);
    TEST_ASSERT_EQUAL(180, testServo->getAngle());
}

void test_should_set_and_read_servo_angle_instantly() {
    testServo->setAngleInstantly(90);
    TEST_ASSERT_EQUAL(90, testServo->getAngle());
    TEST_ASSERT_FALSE(testServo->getIsMoving());
}

void run_servo_tests() {
    RUN_TEST(test_should_set_and_read_servo_angle_instantly);
    RUN_TEST(test_should_clamp_servo_angle_instantly_to_180);
}

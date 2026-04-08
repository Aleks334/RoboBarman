#include "unity.h"
#include "../test_hal_globals.h"
#include "test_sensor.h"

void test_sensor_should_return_simulated_value() {
    testSensor->simulateState(true);
    TEST_ASSERT_TRUE(testSensor->hasDetectedObject());
    
    testSensor->simulateState(false);
    TEST_ASSERT_FALSE(testSensor->hasDetectedObject());
}

void run_sensor_tests() {
    RUN_TEST(test_sensor_should_return_simulated_value);
}

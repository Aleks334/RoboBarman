#include "unity.h"
#include "../test_hal_globals.h"
#include "test_led.h"

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

void run_led_tests() {
    RUN_TEST(test_should_turn_on_led);
    RUN_TEST(test_should_turn_off_led);
    RUN_TEST(test_should_toggle_led_state);
}

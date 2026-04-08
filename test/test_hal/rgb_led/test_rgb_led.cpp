#include "unity.h"
#include "../test_hal_globals.h"
#include "test_rgb_led.h"

void test_should_be__turned_off_after_init() {
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


void run_rgb_led_tests() {
    RUN_TEST(test_should_be__turned_off_after_init);
    RUN_TEST(test_should_change_and_store_color);
    RUN_TEST(test_should_turn_off);
}

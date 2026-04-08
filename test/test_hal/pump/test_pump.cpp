#include "unity.h"
#include "../test_hal_globals.h"
#include "test_pump.h"

const uint32_t POUR_TIME = 500;
const uint32_t TEST_PUMP_START_DELAY_MS = 100;
const uint32_t TEST_PUMP_STOP_DELAY_MS = 100;

void test_should_be_idle_and_not_busy_at_start() {
    TEST_ASSERT_EQUAL(PumpState::IDLE, testPump->getState());
    TEST_ASSERT_FALSE(testPump->isBusy());
}

void test_should_handle_full_pouring_cycle_with_delays() {
    testPump->pour(POUR_TIME, clock);
    testPump->update(clock);

    tickFastForward(TEST_PUMP_START_DELAY_MS - 1);
    testPump->update(clock);
    TEST_ASSERT_EQUAL(PumpState::START_DELAY, testPump->getState());
    TEST_ASSERT_TRUE(testPump->isBusy());
    TEST_ASSERT_FALSE(testPump->isRunning());

    tick(); 
    testPump->update(clock);
    TEST_ASSERT_EQUAL(PumpState::RUNNING, testPump->getState());
    TEST_ASSERT_TRUE(testPump->isRunning());

    tickFastForward(POUR_TIME / 2);
    testPump->update(clock);
    TEST_ASSERT_EQUAL(PumpState::RUNNING, testPump->getState());

    tickFastForward(POUR_TIME / 2);
    testPump->update(clock);
    TEST_ASSERT_EQUAL(PumpState::STOP_DELAY, testPump->getState());
    TEST_ASSERT_FALSE(testPump->isRunning());
    TEST_ASSERT_TRUE(testPump->isBusy());

    tickFastForward(TEST_PUMP_STOP_DELAY_MS);
    testPump->update(clock);
    TEST_ASSERT_EQUAL(PumpState::IDLE, testPump->getState());
    TEST_ASSERT_FALSE(testPump->isBusy());
}

void test_should_ignore_new_pour_requests_while_busy() {
    testPump->pour(POUR_TIME, clock);
    testPump->update(clock);
    
    uint32_t newDuration = 9999;
    testPump->pour(newDuration, clock);
    
    TEST_ASSERT_EQUAL(PumpState::START_DELAY, testPump->getState());
}

void test_should_stop_instantly_ignoring_all_delays() {
    testPump->pour(POUR_TIME, clock);
    tickFastForward(TEST_PUMP_START_DELAY_MS + 10);
    testPump->update(clock);
    TEST_ASSERT_TRUE(testPump->isRunning());

    testPump->stopInstantly();

    TEST_ASSERT_EQUAL(PumpState::IDLE, testPump->getState());
    TEST_ASSERT_FALSE(testPump->isBusy());
}

void run_pump_tests() {
    RUN_TEST(test_should_be_idle_and_not_busy_at_start);
    RUN_TEST(test_should_handle_full_pouring_cycle_with_delays);
    RUN_TEST(test_should_ignore_new_pour_requests_while_busy);
    RUN_TEST(test_should_stop_instantly_ignoring_all_delays);
}

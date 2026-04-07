#include <unity.h>
#include "Barman.h"
#include "Queue.h"
#include <Arduino.h>
#include "../test_common.h"

using namespace TestUtils;

const uint8_t TEST_QUEUE_CAPACITY = 5;

const unsigned long TEST_MOVE_TIME = 50;
const unsigned long TEST_FILLING_TIME = 100;

const uint8_t TEST_PUMP_PIN = 8;
const uint8_t TEST_SERVO_PIN = 9;

uint8_t testStationsDegreeAngles[3] = {30, 90, 150};
const uint8_t TEST_IDLE_POS = 0;

Queue* queue;
Pump* pump;
ServoMotor* servo;
Barman* barman;

void setUp() {
    clock = 0;
    queue = new Queue(TEST_QUEUE_CAPACITY);
    pump = new Pump(TEST_PUMP_PIN);
    servo = new ServoMotor(TEST_SERVO_PIN);
    
    barman = new Barman(*queue, TEST_MOVE_TIME, TEST_FILLING_TIME, *pump, *servo, testStationsDegreeAngles, TEST_IDLE_POS);
    barman->begin();
}

void tearDown() {
    delete barman;
    delete queue;
    delete pump;
    delete servo;
}

void test_should_initialize_in_waiting_state() {
    TEST_ASSERT_EQUAL(BarmanState::WAITING_FOR_TASK, barman->getState());
    TEST_ASSERT_EQUAL(Barman::NO_STATION, barman->getCurrentlyServedStationId());
}

void test_should_transition_to_moving_when_order_received() {
    uint8_t stationId = 1;
    queue->insert(stationId);

    barman->update(clock);
    tick();

    TEST_ASSERT_EQUAL(BarmanState::MOVING, barman->getState());
    TEST_ASSERT_EQUAL(stationId, barman->getCurrentlyServedStationId());
}

void test_should_stay_moving_until_time_elapses() {
    uint8_t stationId = 1;
    queue->insert(stationId);

    barman->update(clock);
    tickFastForward(TEST_MOVE_TIME - 1);
    barman->update(clock);
    tick();

    TEST_ASSERT_EQUAL(BarmanState::MOVING, barman->getState());
}

void test_should_transition_to_filling_after_move_finishes() {
    uint8_t stationId = 1;
    queue->insert(stationId);

    barman->update(clock);
    tickFastForward(TEST_MOVE_TIME);
    barman->update(clock);
    tick(); 

    TEST_ASSERT_EQUAL(BarmanState::FILLING, barman->getState());
}

void test_should_transition_back_to_waiting_after_process_finishes() {
    uint8_t stationId = 1;
    queue->insert(stationId);

    barman->update(clock);
    tickFastForward(TEST_MOVE_TIME);
    barman->update(clock);
    tickFastForward(TEST_FILLING_TIME);
    barman->update(clock);
    tick();

    TEST_ASSERT_TRUE(barman->consumeHasFinishedFillingFlag());
    TEST_ASSERT_EQUAL(BarmanState::WAITING_FOR_TASK, barman->getState());
}

void test_should_set_finished_filling_flag_exactly_once() {
    uint8_t stationId = 1;
    queue->insert(stationId);

    barman->update(clock);
    tickFastForward(TEST_MOVE_TIME);
    barman->update(clock);
    tickFastForward(TEST_FILLING_TIME);
    barman->update(clock);
    tick();

    TEST_ASSERT_TRUE(barman->consumeHasFinishedFillingFlag());
    TEST_ASSERT_FALSE(barman->consumeHasFinishedFillingFlag());
}

void test_should_handle_multiple_stations_in_sequence() {
    uint8_t station1Id = 1;
    uint8_t station2Id = 2;
    queue->insert(station1Id);
    queue->insert(station2Id);

    barman->update(clock);
    tickFastForward(TEST_MOVE_TIME);
    barman->update(clock);
    tickFastForward(TEST_FILLING_TIME);
    barman->update(clock);
    tick();
    barman->update(clock);
    tick();

    TEST_ASSERT_EQUAL(BarmanState::MOVING, barman->getState());
    TEST_ASSERT_EQUAL(station2Id, barman->getCurrentlyServedStationId());
}

void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_should_initialize_in_waiting_state);
    RUN_TEST(test_should_transition_to_moving_when_order_received);
    RUN_TEST(test_should_stay_moving_until_time_elapses);
    RUN_TEST(test_should_transition_to_filling_after_move_finishes);
    RUN_TEST(test_should_transition_back_to_waiting_after_process_finishes);
    RUN_TEST(test_should_set_finished_filling_flag_exactly_once);
    RUN_TEST(test_should_handle_multiple_stations_in_sequence);

    UNITY_END();
}

void loop() {}

#include <unity.h>
#include "Barman.h"
#include "Queue.h"
#include <Arduino.h>

const unsigned long MOVE_TIME = 2000;
const unsigned long FILL_TIME = 3000;
const uint8_t NO_STATION = 255;

Queue* queue;
Barman* barman;

void setUp() {
    queue = new Queue();
    barman = new Barman(*queue, MOVE_TIME, FILL_TIME);
}

void tearDown() {
    delete barman;
    delete queue;
}

void test_should_initialize_in_waiting_state() {
    TEST_ASSERT_EQUAL(BarmanState::WAITING_FOR_TASK, barman->getState());
    TEST_ASSERT_EQUAL(NO_STATION, barman->getCurrentlyServed());
}

void test_should_transition_to_moving_when_order_received() {
    queue->insert(2);
    
    barman->update(1000);
    
    TEST_ASSERT_EQUAL(BarmanState::MOVING, barman->getState());
    TEST_ASSERT_EQUAL(2, barman->getCurrentlyServed());
}

void test_should_stay_moving_until_time_elapses() {
    queue->insert(1);

    barman->update(1000);
    barman->update(1000 + MOVE_TIME - 1); 
    
    TEST_ASSERT_EQUAL(BarmanState::MOVING, barman->getState());
}

void test_should_transition_to_filling_after_move_finishes() {
    queue->insert(1);

    barman->update(1000); 
    barman->update(1000 + MOVE_TIME); 
    
    TEST_ASSERT_EQUAL(BarmanState::FILLING, barman->getState());
}

void test_should_transition_back_to_waiting_after_filling_finishes() {
    queue->insert(1);

    barman->update(1000);             
    barman->update(1000 + MOVE_TIME);
    barman->update(1000 + MOVE_TIME + FILL_TIME); 
    
    TEST_ASSERT_EQUAL(BarmanState::WAITING_FOR_TASK, barman->getState());
}


void test_should_fire_finished_filling_flag_exactly_once() {
    queue->insert(1);
    
    barman->update(0); 
    barman->update(MOVE_TIME); 
    barman->update(MOVE_TIME + FILL_TIME);
    
    TEST_ASSERT_TRUE(barman->hasFinishedFilling());
    TEST_ASSERT_FALSE(barman->hasFinishedFilling());
}

void test_should_handle_multiple_orders_in_sequence() {
   queue->insert(0);
    queue->insert(4);
    
    // order "0"
    barman->update(0);                                   
    barman->update(MOVE_TIME);       
    barman->update(MOVE_TIME + FILL_TIME);
    
    TEST_ASSERT_EQUAL(BarmanState::WAITING_FOR_TASK, barman->getState());
    
    // order "4"
    barman->update(MOVE_TIME + FILL_TIME + 50);
                   
    TEST_ASSERT_EQUAL(BarmanState::MOVING, barman->getState());
    TEST_ASSERT_EQUAL(4, barman->getCurrentlyServed());
}

void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_should_initialize_in_waiting_state);
    RUN_TEST(test_should_transition_to_moving_when_order_received);
    RUN_TEST(test_should_stay_moving_until_time_elapses);
    RUN_TEST(test_should_transition_to_filling_after_move_finishes);
    RUN_TEST(test_should_transition_back_to_waiting_after_filling_finishes);
    RUN_TEST(test_should_fire_finished_filling_flag_exactly_once);
    RUN_TEST(test_should_handle_multiple_orders_in_sequence);

    UNITY_END();
}

void loop() {}

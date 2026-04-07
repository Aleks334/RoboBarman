#include <unity.h>
#include "Station.h"
#include "../test_common.h"
using namespace TestUtils;

Sensor* sensor;
RgbLed* led;
Queue* queue;
Barman* barman;
Station* station;

const unsigned long TEST_MOVE_TIME = 20;
const unsigned long TEST_FILLING_TIME = 30;
const uint16_t TEST_LED_BLINK_INTERVAL = 10;

void setUp() {
    clock = 0;
    sensor = new Sensor(13);
    led = new RgbLed(1, 2, 3);

    queue = new Queue();
    barman = new Barman(*queue, TEST_MOVE_TIME, TEST_FILLING_TIME);
    station = new Station(0, *sensor, *led, *queue, *barman, TEST_LED_BLINK_INTERVAL);
    
    station->begin();
}

void tearDown() {
    delete station; 
    delete barman; 
    delete queue; 
    delete led; 
    delete sensor;
}

void test_should_station_be_idle_initially() {
    TEST_ASSERT_EQUAL(StationState::IDLE, station->getState());
}

void test_should_cycle_through_full_order_process() {
    sensor->simulateState(true);
    station->update(clock);
    barman->update(clock);
    TEST_ASSERT_EQUAL(StationState::OCCUPIED, station->getState());

    tick();
    station->update(clock);
    barman->update(clock);
    TEST_ASSERT_EQUAL(StationState::IN_PROGRESS, station->getState());
   
    tickFastForward(TEST_MOVE_TIME);
    station->update(clock);
    barman->update(clock);
    TEST_ASSERT_EQUAL(StationState::IN_PROGRESS, station->getState());

    tickFastForward(TEST_FILLING_TIME);
    barman->update(clock); // barman needs to set finished filling flag first, then station can consume it
    station->update(clock);
    TEST_ASSERT_EQUAL(StationState::READY, station->getState());

    tick();
    sensor->simulateState(false);
    station->update(clock);
    barman->update(clock);
    TEST_ASSERT_EQUAL(StationState::IDLE, station->getState());
}

void test_should_transition_to_idle_if_cup_removed_when_occupied() {
    sensor->simulateState(true);
    station->update(clock);
    barman->update(clock);
    tick();

    TEST_ASSERT_EQUAL(StationState::OCCUPIED, station->getState());
    
    sensor->simulateState(false);
    station->update(clock);
    barman->update(clock);
    tick();
    
    TEST_ASSERT_EQUAL(StationState::IDLE, station->getState());
}

void test_should_led_be_green_initially() {
    sensor->simulateState(false);
    station->update(clock);
    
    TEST_ASSERT_EQUAL(Color::GREEN, led->getColor());

    tickFastForward(TEST_LED_BLINK_INTERVAL);
    TEST_ASSERT_EQUAL(Color::GREEN, led->getColor()); // doesn't blink
}

void test_should_led_be_red_when_station_is_occupied() {
    sensor->simulateState(true);
    station->update(clock);
    TEST_ASSERT_EQUAL(Color::RED, led->getColor());

    tickFastForward(TEST_LED_BLINK_INTERVAL);
    TEST_ASSERT_EQUAL(Color::RED, led->getColor());  // doesn't blink
}

void test_should_blink_red_when_in_progress() {
    sensor->simulateState(true);
    
    station->update(clock);
    barman->update(clock);
    tick(0);
    station->update(clock);
    tickFastForward(TEST_LED_BLINK_INTERVAL - 1);
    TEST_ASSERT_EQUAL(Color::OFF, led->getColor());
    
    tick();
    station->update(clock);
    TEST_ASSERT_EQUAL(Color::RED, led->getColor());

    tickFastForward(TEST_LED_BLINK_INTERVAL);
    station->update(clock);
    TEST_ASSERT_EQUAL(Color::OFF, led->getColor());
}

void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_should_station_be_idle_initially);
    RUN_TEST(test_should_cycle_through_full_order_process);
    RUN_TEST(test_should_transition_to_idle_if_cup_removed_when_occupied);
    RUN_TEST(test_should_led_be_green_initially);
    RUN_TEST(test_should_led_be_red_when_station_is_occupied);
    RUN_TEST(test_should_blink_red_when_in_progress);

    UNITY_END();
}

void loop() {}

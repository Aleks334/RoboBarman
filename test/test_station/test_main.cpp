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

const uint8_t TEST_PUMP_PIN = 8;
const uint8_t TEST_SERVO_PIN = 9;

uint8_t testStationsDegreeAngles[3] = {30, 90, 150};
const uint8_t TEST_IDLE_POS = 0;

Pump* pump;
ServoMotor* servo;

void setUp() {
    clock = 0;
    queue = new Queue();

    sensor = new Sensor(13);
    led = new RgbLed(1, 2, 3);

    pump = new Pump(TEST_PUMP_PIN);
    servo = new ServoMotor(TEST_SERVO_PIN);
    barman = new Barman(*queue, TEST_MOVE_TIME, TEST_FILLING_TIME, *pump, *servo, testStationsDegreeAngles, TEST_IDLE_POS);
    barman->begin();

    station = new Station(0, *sensor, *led, *queue, *barman, TEST_LED_BLINK_INTERVAL);
    station->begin();
}

void tearDown() {
    delete station; 
    delete led; 
    delete sensor;
    delete barman;
    delete queue;
    delete pump;
    delete servo;
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
    
    tick();
    station->update(clock);
    TEST_ASSERT_EQUAL(StationState::IN_PROGRESS, station->getState());
    TEST_ASSERT_EQUAL(Color::OFF, led->getColor());
    
    tickFastForward(TEST_LED_BLINK_INTERVAL);
    station->update(clock);
    TEST_ASSERT_EQUAL(Color::RED, led->getColor());

    tickFastForward(TEST_LED_BLINK_INTERVAL);
    station->update(clock);
    TEST_ASSERT_EQUAL(Color::OFF, led->getColor());
}

void test_should_blink_green_when_ready() {
    sensor->simulateState(true);
    station->update(clock);
    barman->update(clock);

    tick();
    station->update(clock);
    barman->update(clock);
   
    tickFastForward(TEST_MOVE_TIME);
    station->update(clock);
    barman->update(clock);

    tickFastForward(TEST_FILLING_TIME);
    barman->update(clock); // barman needs to set finished filling flag first, then station can consume it
    station->update(clock);
    TEST_ASSERT_EQUAL(StationState::READY, station->getState());

    
    TEST_ASSERT_EQUAL(Color::OFF, led->getColor());
    tickFastForward(TEST_LED_BLINK_INTERVAL);
    station->update(clock);
    TEST_ASSERT_EQUAL(Color::GREEN, led->getColor());
    tickFastForward(TEST_LED_BLINK_INTERVAL);
    station->update(clock);
    TEST_ASSERT_EQUAL(Color::OFF, led->getColor());
}

void test_should_abort_to_idle_if_cup_removed_during_in_progress() {
    sensor->simulateState(true);
    station->update(clock);
    barman->update(clock);

    tick();
    station->update(clock);
    barman->update(clock);
    TEST_ASSERT_EQUAL(StationState::IN_PROGRESS, station->getState());

    sensor->simulateState(false);
    station->update(clock);
    TEST_ASSERT_EQUAL(StationState::IDLE, station->getState());
}

void test_should_ignore_barman_serving_different_station() {
    uint8_t anotherStationId = 2;
    queue->insert(anotherStationId); 
    barman->update(clock); 
    TEST_ASSERT_EQUAL(anotherStationId, barman->getCurrentlyServedStationId());

    sensor->simulateState(true);
    station->update(clock);
    TEST_ASSERT_EQUAL(StationState::OCCUPIED, station->getState());

    tick();
    barman->update(clock);
    station->update(clock);
    TEST_ASSERT_EQUAL(StationState::OCCUPIED, station->getState()); // station still waits in queue
}

void test_should_stay_idle_if_queue_is_full() {
    uint8_t i = 0;
    while(!queue->isFull()) {
        queue->insert(i + 10); 
    }

    sensor->simulateState(true);
    station->update(clock);
    TEST_ASSERT_EQUAL(StationState::IDLE, station->getState());
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
    RUN_TEST(test_should_blink_green_when_ready);
    RUN_TEST(test_should_abort_to_idle_if_cup_removed_during_in_progress);
    RUN_TEST(test_should_ignore_barman_serving_different_station);
    RUN_TEST(test_should_stay_idle_if_queue_is_full);

    UNITY_END();
}

void loop() {}

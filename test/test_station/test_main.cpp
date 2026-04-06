#include <unity.h>
#include "Station.h"

Sensor* sensor;
Led* led;
Queue* queue;
Barman* barman;
Station* station;

void setUp() {
    sensor = new Sensor(2);
    led = new Led(3);
    queue = new Queue();
    barman = new Barman(*queue);
    station = new Station(0, *sensor, *led, *queue, *barman);
    station->begin();
}

void tearDown() {
    delete station; 
    delete barman;
    delete queue;
    delete led;
    delete sensor;
}

void test_station_should_be_idle_initially() {
    TEST_ASSERT_EQUAL(StationState::IDLE, station->getState());
}

void setup() {
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_station_should_be_idle_initially);
    UNITY_END();
}

void loop() {}

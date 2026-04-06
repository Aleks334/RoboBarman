#include "Station.h"

Station::Station(uint8_t id, Sensor& s, RgbLed& l, Queue& q, Barman& b)
    : id(id), sensor(s), led(l), queue(q), barman(b), 
      state(StationState::IDLE), lastFlashTime(0) {}

void Station::begin() {
    sensor.begin();
    led.begin();
}

void Station::update(unsigned long currentMillis) {
    bool cupDetected = sensor.isActive();

    switch (state) {
        case StationState::IDLE:
            if (cupDetected) {
                if (queue.insert(id) == QueueStatus::OK) {
                    state = StationState::OCCUPIED;
                }
            }
            break;

        case StationState::OCCUPIED:
            if (!cupDetected) {
                state = StationState::IDLE;
              
            } 
            else if (barman.getCurrentlyServed() == id && 
                     barman.getState() == BarmanState::MOVING) {
                state = StationState::IN_PROGRESS;
            }
            break;

        case StationState::IN_PROGRESS:
            if (barman.hasFinishedFilling() && barman.getCurrentlyServed() == id) {
                state = StationState::READY;
            }
            if (!cupDetected) {
                state = StationState::IDLE;
            }
            break;

        case StationState::READY:
            if (!cupDetected) {
                state = StationState::IDLE;
            }
            break;
    }

    updateLed(currentMillis);
}

void Station::updateLed(unsigned long currentMillis) {
    switch (state) {
        case StationState::IDLE:
            led.setColor(Color::GREEN);
            break;

        case StationState::OCCUPIED:
            led.setColor(Color::RED);
            break;

        case StationState::IN_PROGRESS:
            if (currentMillis - lastFlashTime >= FLASH_INTERVAL) {
                static bool toggle = false;
                toggle = !toggle;
                led.setColor(toggle ? Color::RED : Color::OFF);
                lastFlashTime = currentMillis;
            }
            break;

        case StationState::READY:
            if (currentMillis - lastFlashTime >= FLASH_INTERVAL) {
                static bool toggle = false;
                toggle = !toggle;
                led.setColor(toggle ? Color::GREEN : Color::OFF);
                lastFlashTime = currentMillis;
            }
            break;
    }
}

StationState Station::getState() const {
    return state;
}

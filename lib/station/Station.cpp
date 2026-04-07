#include "Station.h"

Station::Station(uint8_t id, Sensor& s, RgbLed& l, Queue& q, Barman& b, uint16_t blinkInterval)
    : id(id), sensor(s), led(l), queue(q), barman(b),
      state(StationState::IDLE), lastBlinkTime(0), blinkInterval(blinkInterval), ledToggleState(false) {}

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
            else if (barman.getCurrentlyServedStationId() == id) {
                state = StationState::IN_PROGRESS;
            }
            break;

        case StationState::IN_PROGRESS:
            if (!cupDetected) {
                state = StationState::IDLE;
            } 

            if (barman.getCurrentlyServedStationId() == id && barman.consumeHasFinishedFillingFlag()) {
                state = StationState::READY;
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
    if ((currentMillis - lastBlinkTime) >= blinkInterval) {
        ledToggleState = !ledToggleState;
        lastBlinkTime = currentMillis;
    }

    switch (state) {
        case StationState::IDLE:
            led.setColor(Color::GREEN);
            break;
        case StationState::OCCUPIED:
            led.setColor(Color::RED);
            break;
        case StationState::IN_PROGRESS:
            led.setColor(ledToggleState ? Color::RED : Color::OFF);
            break;
        case StationState::READY:
            led.setColor(ledToggleState ? Color::GREEN : Color::OFF);
            break;
    }
}

StationState Station::getState() const {
    return state;
}

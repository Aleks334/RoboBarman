#include "Barman.h"
#include <Arduino.h>

Barman::Barman(Queue& orderQueue, uint32_t moveDuration, uint32_t fillDuration, Pump& pump, ServoMotor& servo, const uint8_t* stationsDegreeAngles, uint8_t idlePosition) 
    : queue(orderQueue), 
      currentState(BarmanState::WAITING_FOR_TASK), 
      currentlyServedStationId(NO_STATION), 
      hasFinishedFilling(false),
      moveDuration(moveDuration),
      fillDuration(fillDuration),
      pump(pump),
      servo(servo),
      stationsDegreeAngles(stationsDegreeAngles),
      idlePosition(idlePosition) {}

void Barman::begin() {
    pump.begin();
    servo.begin();
    servo.setAngleInstantly(idlePosition);
}

void Barman::update(unsigned long currentMillis) {
    pump.update(currentMillis);
    servo.update(currentMillis);

    switch (currentState) {
        case BarmanState::WAITING_FOR_TASK:
            if (!queue.isEmpty()) {
                uint8_t stationId;

                if (queue.pop(stationId) == QueueStatus::OK) {
                    currentlyServedStationId = stationId;
                    servo.moveTo(stationsDegreeAngles[currentlyServedStationId], moveDuration, currentMillis);
                    currentState = BarmanState::MOVING;
                }
            }
            break;

        case BarmanState::MOVING:
            if (!servo.getIsMoving()) { 
                pump.pour(fillDuration, currentMillis);
                currentState = BarmanState::FILLING;
            }
            break;

        case BarmanState::FILLING:
            if (!pump.isBusy()) {
                hasFinishedFilling = true;
                currentState = BarmanState::WAITING_FOR_TASK;
            }
            break;
    }
}

void Barman::abort() {
    pump.stopInstantly();
    servo.setAngleInstantly(idlePosition);

    currentState = BarmanState::WAITING_FOR_TASK;
    currentlyServedStationId = NO_STATION;
    hasFinishedFilling = false;
    Serial.println("[ALERT]: Barman service aborted!");
}

bool Barman::consumeHasFinishedFillingFlag() {
    if (hasFinishedFilling) {
        hasFinishedFilling = false;
        return true;
    }
    return false;
}

BarmanState Barman::getState() const {
    return currentState;
}

uint8_t Barman::getCurrentlyServedStationId() const {
    return currentlyServedStationId;
}

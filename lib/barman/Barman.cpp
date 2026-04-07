#include "Barman.h"

Barman::Barman(Queue& orderQueue, uint32_t moveDuration, uint32_t fillDuration, Pump& pump, ServoMotor& servo, const uint8_t* stationsDegreeAngles, uint8_t idlePosition) 
    : queue(orderQueue), 
      currentState(BarmanState::WAITING_FOR_TASK), 
      currentlyServedStationId(NO_STATION), 
      actionStartTime(0),
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
    servo.setAngle(idlePosition);
}

void Barman::update(unsigned long currentMillis) {
    switch (currentState) {
        case BarmanState::WAITING_FOR_TASK:
            if (!queue.isEmpty()) {
                uint8_t stationId;

                if (queue.pop(stationId) == QueueStatus::OK) {
                    currentlyServedStationId = stationId;
                    actionStartTime = currentMillis;

                    servo.setAngle(stationsDegreeAngles[currentlyServedStationId]);
                    currentState = BarmanState::MOVING;
                }
            }
            break;

        case BarmanState::MOVING:
            if ((currentMillis - actionStartTime) >= moveDuration) {
                actionStartTime = currentMillis;

                pump.start();
                currentState = BarmanState::FILLING;
            }
            break;

        case BarmanState::FILLING:
            if ((currentMillis - actionStartTime) >= fillDuration) {
                pump.stop();
                hasFinishedFilling = true; 
                currentState = BarmanState::WAITING_FOR_TASK;
            }
            break;
    }
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

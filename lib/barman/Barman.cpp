#include "Barman.h"

Barman::Barman(Queue& orderQueue) 
    : queue(orderQueue), 
      currentState(BarmanState::WAITING_FOR_TASK), 
      currentlyServed(NO_STATION), 
      actionStartTime(0),
      finishedFillingFlag(false) {}

void Barman::update(unsigned long currentMillis) {
    switch (currentState) {
        case BarmanState::WAITING_FOR_TASK:
            if (!queue.isEmpty()) {
                uint8_t stationIndex;
                if (queue.pop(stationIndex) == QueueStatus::OK) {
                    currentlyServed = stationIndex;
                    actionStartTime = currentMillis;
                    currentState = BarmanState::MOVING;
                }
            }
            break;

        case BarmanState::MOVING:
            if (currentMillis - actionStartTime >= ARM_MOVEMENT_DURATION) {
                actionStartTime = currentMillis;
                currentState = BarmanState::FILLING;
            }
            break;

        case BarmanState::FILLING:
            if (currentMillis - actionStartTime >= FILLING_DURATION) {
                finishedFillingFlag = true; 
                currentState = BarmanState::WAITING_FOR_TASK;
            }
            break;
    }
}

bool Barman::hasFinishedFilling() {
    if (finishedFillingFlag) {
        finishedFillingFlag = false;
        return true;
    }
    return false;
}

BarmanState Barman::getState() const {
    return currentState;
}

uint8_t Barman::getCurrentlyServed() const {
    return currentlyServed;
}
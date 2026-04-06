#include "Barman.h"

Barman::Barman(Queue& orderQueue, uint32_t moveDuration, uint32_t fillDuration) 
    : queue(orderQueue), 
      currentState(BarmanState::WAITING_FOR_TASK), 
      currentlyServed(NO_STATION), 
      actionStartTime(0),
      finishedFillingFlag(false),
      moveDuration(moveDuration),
      fillDuration(fillDuration) {}

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
            if (currentMillis - actionStartTime >= moveDuration) {
                actionStartTime = currentMillis;
                currentState = BarmanState::FILLING;
            }
            break;

        case BarmanState::FILLING:
            if (currentMillis - actionStartTime >= fillDuration) {
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
#pragma once
#include <stdint.h>
#include "Queue.h"

enum class BarmanState : uint8_t {
    WAITING_FOR_TASK,
    MOVING,
    FILLING
};

class Barman {
private:
    Queue& queue;
    BarmanState currentState;
    
    uint8_t currentlyServedStationId; 

    unsigned long actionStartTime;
    bool hasFinishedFilling;

    uint32_t moveDuration;
    uint32_t fillDuration;

public:
    static constexpr uint8_t NO_STATION = 255;

    Barman(Queue& orderQueue, uint32_t moveDuration, uint32_t fillDuration);

    void update(unsigned long currentMillis);

    BarmanState getState() const;
    uint8_t getCurrentlyServedStationId() const;
    bool consumeHasFinishedFillingFlag(); 
};
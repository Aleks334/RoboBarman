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
    
    uint8_t currentlyServed; 
    static constexpr uint8_t NO_STATION = 255;

    unsigned long actionStartTime;
    bool finishedFillingFlag;

    uint32_t moveDuration;
    uint32_t fillDuration;

public:
    Barman(Queue& orderQueue, uint32_t moveDuration, uint32_t fillDuration);

    void update(unsigned long currentMillis);

    BarmanState getState() const;
    uint8_t getCurrentlyServed() const;
    
    bool hasFinishedFilling(); 
};
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

    static constexpr unsigned long ARM_MOVEMENT_DURATION = 2000;
    static constexpr unsigned long FILLING_DURATION = 3000;

public:
    Barman(Queue& orderQueue);

    void update(unsigned long currentMillis);

    BarmanState getState() const;
    uint8_t getCurrentlyServed() const;
    
    bool hasFinishedFilling(); 
};
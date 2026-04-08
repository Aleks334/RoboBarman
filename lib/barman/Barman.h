#pragma once
#include <Arduino.h>
#include "Queue.h"
#include "Pump.h"
#include "ServoMotor.h"

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

    bool hasFinishedFilling;

    uint32_t moveDuration;
    uint32_t fillDuration;

    Pump& pump;
    ServoMotor& servo;

    const uint8_t* stationsDegreeAngles;
    uint8_t idlePosition;

public:
    static constexpr uint8_t NO_STATION = 255;

    Barman(Queue& orderQueue, uint32_t moveDuration, uint32_t fillDuration, Pump& pump, ServoMotor& servo, const uint8_t* stationsDegreeAngles, uint8_t idlePosition);

    void begin();
    void update(unsigned long currentMillis);
    void abort();

    BarmanState getState() const;
    uint8_t getCurrentlyServedStationId() const;
    bool consumeHasFinishedFillingFlag(); 
};
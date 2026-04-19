#include "Sensor.h"

Sensor::Sensor(uint8_t trigPin, uint8_t echoPin, uint16_t debounceDelayMs, uint8_t thresholdCm) 
    : trigPin(trigPin), echoPin(echoPin), 
      debounceDelay(debounceDelayMs), detectionThresholdCm(thresholdCm),
      lastRawState(false), debouncedState(false), lastDebounceTime(0), lastPingTime(0),
      isSimulationMode(false), simulatedState(false) {}

void Sensor::begin() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(trigPin, LOW);
    
    lastRawState = readSensorValue();
    debouncedState = lastRawState;
}

void Sensor::update(unsigned long currentMillis) {
    if (isSimulationMode) return;

    if (currentMillis - lastPingTime < MIN_PING_INTERVAL_MS) {
        return; 
    }
    lastPingTime = currentMillis;

    bool currentState = readSensorValue();

    if (currentState != lastRawState) {
        lastDebounceTime = currentMillis;
    }

    if ((currentMillis - lastDebounceTime) > debounceDelay) {
        debouncedState = currentState;
    }

    lastRawState = currentState;
}

bool Sensor::hasDetectedObject() const {
    return isSimulationMode ? simulatedState : debouncedState;
}

void Sensor::simulateState(bool state) {
    isSimulationMode = true;
    simulatedState = state;
}

void Sensor::disableSimulation() {
    isSimulationMode = false;
}

bool Sensor::readSensorValue() {
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(TRIGGER_PULSE_US);
    digitalWrite(trigPin, LOW);

    unsigned long echoDurationUs = pulseIn(echoPin, HIGH, PING_TIMEOUT_US); 

    if (echoDurationUs == 0) {
        return false; 
    }

    double distance = calculateDistance(echoDurationUs);

    return (distance >= 0.0 && distance <= detectionThresholdCm);
}

double Sensor::calculateDistance(unsigned long echoDurationUs) const {
    // The speed of sound in air at 20 C is about 343 m/s = 0.0343 cm/us
    const double speedOfSoundCmPerUs = 0.0343;

    double totalDistance = echoDurationUs * speedOfSoundCmPerUs;
    double distanceToObject = totalDistance / 2.0;

    return distanceToObject;
}

#include "HardwareTester.h"

HardwareTester::HardwareTester(
    uint8_t numStations, 
    const StationPins* stationConfigs, 
    uint8_t pinPump, 
    uint32_t pumpStartDelay, 
    uint32_t pumpStopDelay, 
    uint8_t pinServo, 
    bool ledCommonAnode, 
    uint16_t sensorDebounceMs, 
    uint16_t sensorThresholdCm,
    uint32_t moveDurationMs,
    uint32_t fillDurationMs
) : numStations(numStations), 
    pumpStartDelay(pumpStartDelay), 
    pumpStopDelay(pumpStopDelay),
    moveDurationMs(moveDurationMs), 
    fillDurationMs(fillDurationMs) 
{
    pump = new Pump(pinPump, pumpStartDelay, pumpStopDelay);
    servo = new ServoMotor(pinServo);

    stationSensors = new Sensor*[numStations];
    stationLeds = new RgbLed*[numStations];

    for (uint8_t i = 0; i < numStations; i++) {
        StationPins config = stationConfigs[i];
        
        stationSensors[i] = new Sensor(
            config.sensorTrigPin, 
            config.sensorEchoPin, 
            sensorDebounceMs, 
            sensorThresholdCm
        );

        stationLeds[i] = new RgbLed(
            config.red, 
            config.green, 
            config.blue, 
            ledCommonAnode
        );
    }
}

HardwareTester::~HardwareTester() {
    delete pump;
    delete servo;

    for (uint8_t i = 0; i < numStations; i++) {
        delete stationSensors[i];
        delete stationLeds[i];
    }
    
    delete[] stationSensors;
    delete[] stationLeds;
}

void HardwareTester::begin() {
    Serial.begin(9600);
    Serial.println("\n=== HARDWARE INITIALIZATION ===");

    pump->begin();
    servo->begin();

    for (uint8_t i = 0; i < numStations; i++) {
        stationSensors[i]->begin();
        stationLeds[i]->begin();
    }
    
    Serial.println("=== INITIALIZATION SUCCEEDED ===");
}

void HardwareTester::tick(unsigned long durationMs) {
    unsigned long startTime = millis();
    while (millis() - startTime < durationMs) {
        unsigned long currentMillis = millis();
        
        pump->update(currentMillis);
        servo->update(currentMillis);
        for (uint8_t i = 0; i < numStations; i++) {
            stationSensors[i]->update(currentMillis);
        }
    }
}

void HardwareTester::runDiagnostics() {
    Serial.println("\n=== FULL COMPONENT DIAGNOSTICS STARTED ===");

    Serial.println("[TEST] Servomotor");
    servo->setAngleInstantly(60);
    tick(moveDurationMs);
    servo->setAngleInstantly(120);
    tick(moveDurationMs);

    Serial.println("[TEST] Pump");
    unsigned long testStartTime = millis();
    pump->pour(fillDurationMs, testStartTime);
    tick(fillDurationMs + pumpStartDelay + pumpStopDelay + 500);

    for (uint8_t i = 0; i < numStations; i++) {
        Serial.print("\n=== TESTING STATION NO. ");
        Serial.print(i + 1);
        Serial.println(" ===");

        Serial.println("[TEST] RGB LED");
        stationLeds[i]->setColor(Color::RED);
        delay(800);
        stationLeds[i]->setColor(Color::GREEN);
        delay(800);
        stationLeds[i]->setColor(Color::BLUE);
        delay(800);
        stationLeds[i]->off();

        Serial.println("[TEST] Ultrasonic sensor");
        Serial.println("Place the cup in 10 seconds or less...");
        
        unsigned long sensorTestStartTime = millis();
        bool objectDetected = false;

        while (millis() - sensorTestStartTime < 10000) {
            tick(50);
            
            if (stationSensors[i]->hasDetectedObject()) {
                objectDetected = true;
                break;
            }
        }

        if (objectDetected) {
            Serial.println("SUCCESS. Object was detected");
            stationLeds[i]->setColor(Color::GREEN);
            tick(1000);
            stationLeds[i]->off();
        } else {
            Serial.println("ERROR. Object wasn't detected");
            stationLeds[i]->setColor(Color::RED);
            tick(1000);
            stationLeds[i]->off();
        }
    }

    Serial.println("\n=== FULL COMPONENT DIAGNOSTICS COMPLETED ===");
}
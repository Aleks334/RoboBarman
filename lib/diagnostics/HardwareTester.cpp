#include "HardwareTester.h"

using namespace GlobalConfig;

HardwareTester::HardwareTester() {
    pump = new Pump(PIN_PUMP, PUMP_START_DELAY, PUMP_STOP_DELAY);
    servo = new ServoMotor(PIN_SERVO);

    for (uint8_t i = 0; i < NUM_STATIONS; i++) {
        StationPins config = STATIONS_CONFIG[i];
        
        stationSensors[i] = new Sensor(
            config.sensorTrigPin, 
            config.sensorEchoPin, 
            SENSOR_DEBOUNCE_MS, 
            SENSOR_DETECTION_TRESHOLD_CM
        );

        stationLeds[i] = new RgbLed(
            config.red, 
            config.green, 
            config.blue, 
            LED_COMMON_ANODE
        );
    }
}

HardwareTester::~HardwareTester() {
    delete pump;
    delete servo;

    for (uint8_t i = 0; i < GlobalConfig::NUM_STATIONS; i++) {
        delete stationSensors[i];
        delete stationLeds[i];
    }
}

void HardwareTester::begin() {
    Serial.begin(9600);
    Serial.println("\n=== HARDWARE INITIALIZATION ===");

    pump->begin();
    servo->begin();

    for (uint8_t i = 0; i < GlobalConfig::NUM_STATIONS; i++) {
        stationSensors[i]->begin();
        stationLeds[i]->begin();
    }
    
    Serial.println("=== INITIALIZATION SUCCEDED ===");
}

void HardwareTester::tick(unsigned long durationMs) {
    unsigned long startTime = millis();
    while (millis() - startTime < durationMs) {
        unsigned long currentMillis = millis();
        
        pump->update(currentMillis);
        servo->update(currentMillis);
        for (uint8_t i = 0; i < NUM_STATIONS; i++) {
            stationSensors[i]->update(currentMillis);
        }
    }
}

void HardwareTester::runFullDiagnostics() {
    Serial.println("\n=== FULL COMPONENT DIAGNOSTICS STARTED ===");

    Serial.println("[TEST] Servomotor");
    servo->setAngleInstantly(60);
    tick(MOVE_DURATION_MS);
    servo->setAngleInstantly(120);
    tick(MOVE_DURATION_MS);

    Serial.println("[TEST] Pump");
    unsigned long testStartTime = millis();
    pump->pour(FILL_DURATION_MS, testStartTime);
    tick(FILL_DURATION_MS + PUMP_START_DELAY + PUMP_STOP_DELAY + 500);

    for (uint8_t i = 0; i < GlobalConfig::NUM_STATIONS; i++) {
        Serial.print("\n=== TESTING STATION NO. ");
        Serial.print(i + 1);
        Serial.println(" ===");

        Serial.println("[TEST] rgb led");
        stationLeds[i]->setColor(Color::RED);
        delay(800);
        stationLeds[i]->setColor(Color::GREEN);
        delay(800);
        stationLeds[i]->setColor(Color::BLUE);
        delay(800);
        stationLeds[i]->off();

        Serial.println("[TEST] Ultrasonic sensor");
        Serial.println("Place the cup in 10 seconds or less...");
        
        unsigned long testStartTime = millis();
        bool objectDetected = false;

        while (millis() - testStartTime < 10000) {
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
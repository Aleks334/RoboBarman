#include <Arduino.h>
#include "GlobalConfig.h"
#include "Queue.h"
#include "Pump.h"
#include "ServoMotor.h"
#include "Sensor.h"
#include "RgbLed.h"
#include "Barman.h"
#include "Station.h"

using namespace GlobalConfig;

Queue ordersQueue(GlobalConfig::QUEUE_CAPACITY);

Pump barmanPump(PIN_PUMP);
ServoMotor barmanServo(PIN_SERVO);

Barman barman(
    ordersQueue, 
    MOVE_DURATION_MS, 
    FILL_DURATION_MS, 
    barmanPump, 
    barmanServo, 
    SERVO_STATIONS_DEGREE_ANGLES, 
    SERVO_POS_IDLE
);

Station* stations[NUM_STATIONS];

void setup() {
    Serial.begin(9600);
    Serial.println(F("=== AUTOMATIC BARMAN SYSTEM STARTING... ==="));

    barman.begin();
    Serial.println(F("Pump and servomotor initialized."));

    for (uint8_t stationId = 0; stationId < NUM_STATIONS; stationId++) {
        StationPins config = STATIONS_CONFIG[stationId];

        Sensor* sensor = new Sensor(config.sensor, SENSOR_DEBOUNCE_MS);
        RgbLed* led = new RgbLed(config.red, config.green, config.blue, LED_COMMON_ANODE);
        
        stations[stationId] = new Station(
            stationId, 
            *sensor, 
            *led, 
            ordersQueue, 
            barman, 
            LED_BLINK_INTERVAL_MS
        );
        
        stations[stationId]->begin();

        Serial.print("System: Station "); 
        Serial.print(stationId); 
        Serial.println(" ready.");
    }

    Serial.println(F("=== SYSTEM READY TO SERVE ==="));
}

void loop() {
    unsigned long currentMillis = millis();

    barman.update(currentMillis);

    for (uint8_t i = 0; i < NUM_STATIONS; i++) {
        stations[i]->update(currentMillis);
    }
}

#include "ServoCalibrator.h"

using namespace GlobalConfig;

ServoCalibrator::ServoCalibrator() {
    servo = new ServoMotor(PIN_SERVO);

    StationPins config = STATIONS_CONFIG[0];
    sensor = new Sensor(
        config.sensorTrigPin, 
        config.sensorEchoPin, 
        SENSOR_DEBOUNCE_MS, 
        SENSOR_DETECTION_TRESHOLD_CM
    );

    led = new RgbLed(
        config.red, 
        config.green, 
        config.blue, 
        LED_COMMON_ANODE
    );
}

ServoCalibrator::~ServoCalibrator() {
    delete servo;
    delete sensor;
    delete led;
}

void ServoCalibrator::update() {
    unsigned long currentMillis = millis();
    sensor->update(currentMillis);
    servo->update(currentMillis);

    if (locked) return;
    
    if (sensor->hasDetectedObject()) {
        led->setColor(Color::WHITE);
        delay(2000); 
        locked = true;
        blinkResult(currentAngle);
    } else {
        unsigned long lastStep = 0;

        if (currentMillis - lastStep > 100) {
            currentAngle++;
            servo->setAngleInstantly(currentAngle);
            lastStep = currentMillis;
        }
    }
}

void ServoCalibrator::blinkResult(int angle) {
    int hundreds = angle / 100;
    int tens = (angle % 100) / 10;
    int units = angle % 10;

    auto flash = [&](Color color, int count) {
        for (int i = 0; i < count; i++) {
            led->setColor(color); delay(300);
            led->off(); delay(300);
        }
        delay(1000);
    };

    while (true) {
        flash(Color::RED, hundreds);
        flash(Color::GREEN, tens);
        flash(Color::BLUE, units);
        delay(3000);
    }
}
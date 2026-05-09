#include "ServoCalibrator.h"

ServoCalibrator::ServoCalibrator(
    uint8_t servoPin, 
    uint8_t trigPin,
    uint8_t echoPin, 
    uint8_t rPin,
    uint8_t gPin,
    uint8_t bPin,
    uint16_t sensorDebounceMs, 
    uint16_t sensorThresholdCm, 
    bool ledCommonAnode
) {
    servo = new ServoMotor(servoPin);

    sensor = new Sensor(
        trigPin, 
        echoPin, 
        sensorDebounceMs, 
        sensorThresholdCm
    );

    led = new RgbLed(
        rPin, 
        gPin, 
        bPin, 
        ledCommonAnode
    );
}

ServoCalibrator::~ServoCalibrator() {
    delete servo;
    delete sensor;
    delete led;
}

void ServoCalibrator::begin() {
    Serial.begin(9600);
    Serial.println("\n=== SERVO CALIBRATION ===");

    servo->begin();
    sensor->begin();
    led->begin();
    
    servo->setAngleInstantly(currentAngle);
}

void ServoCalibrator::update() {
    unsigned long currentMillis = millis();

    sensor->update(currentMillis);
    servo->update(currentMillis);

    if (locked) return;
    
    if (sensor->hasDetectedObject()) {
        locked = true;
        led->setColor(Color::WHITE);
        blinkResult(currentAngle);
    } else {
        if (currentMillis - lastStep >= 100) {
            currentAngle++;
            if (currentAngle > 160) currentAngle = 20;
            
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
            led->setColor(color);
            delay(300);
            led->off();
            delay(300);
        }
        delay(1000);
    };

    Serial.print("Calibration completed. Angle: ");
    Serial.println(angle);

    while (true) {
        flash(Color::RED, hundreds);
        flash(Color::GREEN, tens);
        flash(Color::BLUE, units);
        delay(3000);
    }
}
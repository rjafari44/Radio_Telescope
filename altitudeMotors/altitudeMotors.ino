#include <Arduino.h>

// motor driver pins
const int M1_INA{2};
const int M1_INB{4};
const int M1_PWM{9};
const int M1_EN {6};   // EN/DIAG

const int M2_INA{7};
const int M2_INB{8};
const int M2_PWM{10};
const int M2_EN {12};  // EN/DIAG

// joystick
const int readStick{15};
const int center{512};
const int deadZone{20};
const int maxSpeed{140};

int stickValue{0};
int pwmValue{0};
int delta{};


void setup() {
    pinMode(M1_INA, OUTPUT);
    pinMode(M1_INB, OUTPUT);
    pinMode(M1_PWM, OUTPUT);

    pinMode(M2_INA, OUTPUT);
    pinMode(M2_INB, OUTPUT);
    pinMode(M2_PWM, OUTPUT);

    // EN/DIAG pins: open-drain → pull-up
    pinMode(M1_EN, INPUT_PULLUP);
    pinMode(M2_EN, INPUT_PULLUP);
}

void loop() {
    stickValue = analogRead(readStick);
    delta = stickValue - center;

    // stop
    if (abs(delta) < deadZone) {
        analogWrite(M1_PWM, 0);
        analogWrite(M2_PWM, 0);

        // coast to stop
        digitalWrite(M1_INA, LOW);
        digitalWrite(M1_INB, LOW);
        digitalWrite(M2_INA, LOW);
        digitalWrite(M2_INB, LOW);
    }

    // Forward
    else if (delta > 0) {
        pwmValue = map(delta, deadZone, 512, 0, 255);

        if (pwmValue > maxSpeed) {
            pwmValue = maxSpeed;
        }

        digitalWrite(M1_INA, HIGH);
        digitalWrite(M1_INB, LOW);
        digitalWrite(M2_INA, HIGH);
        digitalWrite(M2_INB, LOW);

        analogWrite(M1_PWM, pwmValue);
        analogWrite(M2_PWM, pwmValue);
    }

    // Reverse
    else {
        pwmValue = map(-delta, deadZone, 512, 0, 255);

        if (pwmValue > maxSpeed) {
            pwmValue = maxSpeed;
        }

        digitalWrite(M1_INA, LOW);
        digitalWrite(M1_INB, HIGH);
        digitalWrite(M2_INA, LOW);
        digitalWrite(M2_INB, HIGH);

        analogWrite(M1_PWM, pwmValue);
        analogWrite(M2_PWM, pwmValue);
    }

    // fault check
    if (digitalRead(M1_EN) == LOW || digitalRead(M2_EN) == LOW) {

        analogWrite(M1_PWM, 0);
        analogWrite(M2_PWM, 0);
    }
}

#include <Arduino.h>

// set motor driver pins of integer type
const int M1_INA {2};
const int M1_INB {4};
const int M1_PWM {9};
const int M1_EN {6};
const int M2_INA {7};
const int M2_INB {8};
const int M2_PWM {10};
const int M2_EN {12};

const int readStick{15};
int stickValue{};
int pwmValue{};
const int deadZone{20};


void setup() {
    pinMode(M1_INA, OUTPUT);
    pinMode(M1_INB, OUTPUT);
    pinMode(M1_PWM, OUTPUT);
    pinMode(M2_INA, OUTPUT);
    pinMode(M2_INB, OUTPUT);
    pinMode(M2_PWM, OUTPUT);
    pinMode(M1_EN, OUTPUT);
    pinMode(M2_EN, OUTPUT);
    digitalWrite(M1_EN, HIGH);
    digitalWrite(M2_EN, HIGH);

}

void loop() {
    stickValue = analogRead(readStick);
    pwmValue = map(stickValue, 0, 1023, 0, 255);

    if (stickValue > 512 + deadZone) {
        digitalWrite(M1_INA, HIGH);
        digitalWrite(M1_INB, LOW);
        digitalWrite(M2_INA, HIGH);
        digitalWrite(M2_INB, LOW);
        analogWrite(M1_PWM, pwmValue);
        analogWrite(M2_PWM, pwmValue);
    }

    else if (stickValue < 512 - deadZone) {
        digitalWrite(M1_INA, LOW);
        digitalWrite(M1_INB, HIGH);
        digitalWrite(M2_INA, LOW);
        digitalWrite(M2_INB, HIGH);
        analogWrite(M1_PWM, pwmValue);
        analogWrite(M2_PWM, pwmValue);
    }
    else {
        analogWrite(M1_PWM, 0);
        analogWrite(M2_PWM, 0);
    }
    
}
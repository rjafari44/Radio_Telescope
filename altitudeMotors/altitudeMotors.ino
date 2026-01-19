#include <Arduino.h>
#include "myheader.h"

// variables for calculating joystick value
int stickValue{};
int pwmValue{};
int delta{}; // variable for calculating position

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
    stickValue = analogRead(READSTICK); // get value from joystick between 0-1023
    delta = stickValue - CENTER;        // delta is difference of the joystick value (0-1023) - 512 (center)

    // stop, if delta is less than 20, it is a safety stop range
    if (abs(delta) < DEADZONE) {
        stopMotors();
    }

    // Forward if delta is greater than 0 (conditionally it's value has to be more than 20)
    else if (delta > 0) {

        // the map function works by the equation: y=(x−in_min)∗(out_max−out_min)/(in_max−in_min)+out_min where 
        // long map(long x, long in_min, long in_max, long out_min, long out_max);
        // it converts an analog value smoothly to pwm
        pwmValue = map(delta, DEADZONE, 512, 0, 255); 

        if (pwmValue > MAXSPEED) {
            pwmValue = MAXSPEED;
        }

        forwardMotors(pwmValue);
    }

    // Reverse if delta is less than 0 (conditionally its absolute value has to be greater than 20)
    else {
        // look at explanation in above else if
        pwmValue = map(-delta, DEADZONE, 512, 0, 255);

        if (pwmValue > MAXSPEED) {
            pwmValue = MAXSPEED;
        }

        reverseMotors(pwmValue);
    }

    // fault check
    if (digitalRead(M1_EN) == LOW || digitalRead(M2_EN) == LOW) {
        stopMotors();
    }

    // --- Stall detection for both motors ---
    if (checkStall(M1_CS) || checkStall(M2_CS)) {
        stopMotors();
    }
}

#include <Arduino.h>

// motor driver pins
const int M1_INA{2};
const int M1_INB{4};
const int M1_PWM{9};
const int M1_EN {6};  // EN/DIAG sense for motor 1
const int M1_CS {14}; // current sense for motor 1

const int M2_INA{7};
const int M2_INB{8};
const int M2_PWM{10};
const int M2_EN {12}; // EN/DIAG for motor 2
const int M2_CS {15}; // current sense for motor 2

// joystick
const int readStick{16};
const int center{512};
const int deadZone{20};
const int maxSpeed{140};

// variables for calculating joystick value
int stickValue{0};
int pwmValue{0};
int delta{}; // variable for calculating position

const float CURRENT_SENSITIVITY = 0.26; 
const float STALL_CURRENT = 2.0; 

float readCurrent(int csPin) {
    int sensorValue=analogRead(csPin);
    float voltage = sensorValue * (5.0 / 1023.0);
    return voltage / CURRENT_SENSITIVITY;
}


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
    stickValue = analogRead(readStick); // get value from joystick between 0-1023
    delta = stickValue - center;        // delta is difference of the joystick value (0-1023) - 512 (center)

    // stop, if delta is less than 20, it is a safety stop range
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

        // the map function works by the equation: y=(x−in_min)∗(out_max−out_min)/(in_max−in_min)+out_min where 
        // long map(long x, long in_min, long in_max, long out_min, long out_max);
        // it converts an analog value smoothly to pwm
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
        // look at explanation in above else if
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

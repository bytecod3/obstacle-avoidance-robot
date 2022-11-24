#include <Arduino.h>
#include <pins.h>
#include "fast_ping.h"

uint8_t d_flag;
FastPing *pinger;

int my_delay = 50;

// speed constants
int speed = 100;
int turn_speed = 90;

// motor movement function prototypes
void forward(int, int);

void left(int, int);

void right(int, int);

void reverse(int, int);

void stop();

// ==============================Motor functions ====================================
void forward(int r_speed, int l_speed) {

    // turn ON all motors
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    // left motor on
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    analogWrite(ena, l_speed);
    analogWrite(enb, r_speed);
}

void left(int r_speed, int l_speed) {
    // turn motors left - right motor on, left motor reversed direction
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    analogWrite(ena, turn_speed);
    analogWrite(enb, r_speed);
}

void right(int r_speed, int l_speed) {
    // turn motors right
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    analogWrite(ena, l_speed);
    analogWrite(enb, turn_speed);
}

void reverse(int r_speed, int l_speed) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    // left motor on
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    analogWrite(ena, l_speed);
    analogWrite(enb, r_speed);
}

void u_turn(int r_speed, int l_speed) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    analogWrite(ena, l_speed);
    analogWrite(enb, turn_speed);

    delayMicroseconds(3000);
}

void stop() {
    // turn motors OFF
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

void initialize_motors() {
    /*
    Initialize the motors
    */

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
}

void setup() {
    // set up motor driver pins
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(ena, OUTPUT);
    pinMode(enb, OUTPUT);

    pinger = new FastPing(COMMON_TRIG, LEFT_ECHO, RIGHT_ECHO, FRONT_ECHO);

    // init motors
    // initialize_motors();
    delayMicroseconds(2000);

    Serial.begin(9600);
    right(speed, speed);
    delay(my_delay);
    stop();
}

void steer(direction_t dir){
    switch (dir) {
        case FRONT:
            forward(speed, speed);
            Serial.println("FRONT");
            break;
        case RIGHT:
            right(speed, speed);
            Serial.println("RIGHT");
            break;
        case LEFT:
            left(speed, speed);
            Serial.println("LEFT");
            break;
        case U_TURN:
            u_turn(speed, speed);
            Serial.println("U-TURN");
            break;
    }
}

void loop() {
    d_flag = pinger->waitForAll();

    switch (d_flag) {
        case U_TURN:
        case FRONT:
        case RIGHT:
        case LEFT:
            // steer to that specific side
            steer((direction_t)(d_flag));
            break;
        case RIGHT | LEFT:
            // go to side with most space
            steer((direction_t)pinger->greatest());
            break;
        case RIGHT | FRONT:
        case FRONT | LEFT:
        case FRONT | LEFT | RIGHT:
            // prefer front
            steer((direction_t)FRONT);
            break;
        default:
            // go to side with most space
            // we should not get here
            steer((direction_t)pinger->greatest());
    }
    delay(my_delay);
}
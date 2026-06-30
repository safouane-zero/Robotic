#ifndef HEADER_HPP
#define HEADER_HPP

#include <Arduino.h>
#include <Ultrasonic.h>
#include <Servo.h>

typedef enum {
    RELEASE,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
} MotorDirection;

int LDR(uint8_t pin);
void configure_LDR(uint8_t pin);
void configure_motor_pins(int en_pin, int inA_pin, int inB_pin);
void set_motor(int en_pin, int inA_pin, int inB_pin, int speed, MotorDirection dir);
void turn_motor(int ena, int in1, int in2, int enb, int in3, int in4, int speed, MotorDirection direction);


#endif
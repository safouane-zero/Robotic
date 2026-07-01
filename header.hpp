#ifndef HEADER_HPP
#define HEADER_HPP

#include <Arduino.h>

enum MotorDirection {
    RELEASE,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    SOFT_LEFT,
    SOFT_RIGHT
};

struct IRPinState {
    uint8_t R9;
    uint8_t R10;
    uint8_t R11;
    uint8_t R12;
    uint8_t IR_LEFT;
    uint8_t IR_RIGHT;
};

// Motor control functions
void configure_motor_pins(uint8_t en_pin, uint8_t in1_pin, uint8_t in2_pin);
void set_motor(uint8_t en_pin, uint8_t in1_pin, uint8_t in2_pin, uint8_t speed, MotorDirection dir);
void drive_robot(uint8_t ena, uint8_t in1, uint8_t in2, uint8_t enb, uint8_t in3, uint8_t in4, uint8_t speed, MotorDirection direction);

// Sensor functions
void configure_IR(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4, uint8_t left_ir, uint8_t right_ir);
IRPinState read_IR_inputs(uint8_t r9, uint8_t r10, uint8_t r11, uint8_t r12, uint8_t left_ir, uint8_t right_ir);
void configure_LDR(uint8_t pin);
int readLDR(uint8_t pin);

#endif
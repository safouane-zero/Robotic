#ifndef HEADER_HPP
#define HEADER_HPP

#include <Arduino.h>

enum MotorDirection {
    RELEASE,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

struct IRPinState {
    uint8_t far_left;
    uint8_t center_left;
    uint8_t center_right;
    uint8_t far_right;
};

// Motor control functions
void configure_motor_pins(uint8_t en_pin, uint8_t in1_pin, uint8_t in2_pin);
void set_motor(uint8_t en_pin, uint8_t in1_pin, uint8_t in2_pin, uint8_t speed, MotorDirection dir);
void drive_robot(uint8_t ena, uint8_t in1, uint8_t in2, uint8_t enb, uint8_t in3, uint8_t in4, uint8_t speed, MotorDirection direction);

// Sensor functions
void configure_IR(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4);
IRPinState read_IR_inputs(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4);
void configure_LDR(uint8_t pin);
int readLDR(uint8_t pin);

#endif
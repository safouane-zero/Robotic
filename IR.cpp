#include "header.hpp"

void configure_IR(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4) {
    pinMode(in1, INPUT);
    pinMode(in2, INPUT);
    pinMode(in3, INPUT);
    pinMode(in4, INPUT);
}

IRPinState read_IR_inputs(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4) {
    IRPinState pins;
    pins.far_left = digitalRead(in1);
    pins.center_left = digitalRead(in2);
    pins.center_right = digitalRead(in3);
    pins.far_right = digitalRead(in4);
    
    return pins;
}
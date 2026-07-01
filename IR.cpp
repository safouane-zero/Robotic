#include "header.hpp"

void configure_IR(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4, uint8_t left_ir, uint8_t right_ir) {
    pinMode(in1, INPUT);
    pinMode(in2, INPUT);
    pinMode(in3, INPUT);
    pinMode(in4, INPUT);
    pinMode(left_ir, INPUT);
    pinMode(right_ir, INPUT);
}

IRPinState read_IR_inputs(uint8_t r9, uint8_t r10, uint8_t r11, uint8_t r12, uint8_t left_ir, uint8_t right_ir){
    IRPinState pins;
    pins.R9 = digitalRead(r9);
    pins.R10 = digitalRead(r10);
    pins.R11 = digitalRead(r11);
    pins.R12 = digitalRead(r12);
    pins.IR_LEFT = digitalRead(left_ir);
    pins.IR_RIGHT = digitalRead(right_ir);
    
    return pins;
}
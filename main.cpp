#include "header.hpp"

#define BLACK 1
#define WHITE 0

const uint8_t ENA = 9;
const uint8_t IN1 = 8;
const uint8_t IN2 = 7;
const uint8_t ENB = 3;
const uint8_t IN3 = 5;
const uint8_t IN4 = 4;

const uint8_t R9 = 10;
const uint8_t R10 = 11;
const uint8_t R11 = 12;
const uint8_t R12 = 13;
const uint8_t LEFT_IR = 2;
const uint8_t RIGHT_IR = 1;

void setup() {
    Serial.begin(9600);
    configure_motor_pins(ENA, IN1, IN2);
    configure_motor_pins(ENB, IN3, IN4);
    configure_IR(R9, R10, R11, R12, LEFT_IR, RIGHT_IR);
    delay(3000);
    
}

void loop() {
    IRPinState ir = read_IR_inputs(R9, R10, R11, R12, LEFT_IR, RIGHT_IR);
}
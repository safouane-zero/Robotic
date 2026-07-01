#include "header.hpp"

void configure_motor_pins(uint8_t en_pin, uint8_t in1_pin, uint8_t in2_pin) {
    pinMode(en_pin, OUTPUT);
    pinMode(in1_pin, OUTPUT);
    pinMode(in2_pin, OUTPUT);
}

void set_motor(uint8_t en_pin, uint8_t in1_pin, uint8_t in2_pin, uint8_t speed, MotorDirection dir) {
    analogWrite(en_pin, speed);
    
    if (dir == FORWARD) {
        digitalWrite(in1_pin, LOW);
        digitalWrite(in2_pin, HIGH);
    } else if (dir == BACKWARD) {
        digitalWrite(in1_pin, HIGH);
        digitalWrite(in2_pin, LOW);
    } else {
        digitalWrite(in1_pin, LOW);
        digitalWrite(in2_pin, LOW);
    }
}

void drive_robot(uint8_t ena, uint8_t in1, uint8_t in2, uint8_t enb, uint8_t in3, uint8_t in4, uint8_t speed, MotorDirection direction) {
    switch(direction) {
        case FORWARD:
            set_motor(ena, in1, in2, speed, FORWARD);
            set_motor(enb, in3, in4, speed, FORWARD);
            break;
        case BACKWARD:
            set_motor(ena, in1, in2, speed, BACKWARD);
            set_motor(enb, in3, in4, speed, BACKWARD);
            break;
        case LEFT:
            set_motor(ena, in1, in2, speed, BACKWARD);
            set_motor(enb, in3, in4, speed, FORWARD);
            break;
        case RIGHT:
            set_motor(ena, in1, in2, speed, FORWARD);
            set_motor(enb, in3, in4, speed, BACKWARD);
            break;
        case SOFT_LEFT: {
            int left_speed  = constrain(speed * 0.80, 0, 255);
            int right_speed = constrain(speed * 1.20, 0, 255);
            
            set_motor(ena, in1, in2, left_speed, FORWARD);
            set_motor(enb, in3, in4, right_speed, FORWARD);
            break;
        }
        case SOFT_RIGHT: {
            int left_speed  = constrain(speed * 1.20, 0, 255);
            int right_speed = constrain(speed * 0.80, 0, 255);
            
            set_motor(ena, in1, in2, left_speed, FORWARD);
            set_motor(enb, in3, in4, right_speed, FORWARD);
            break;
        }
        case RELEASE:
            set_motor(ena, in1, in2, 0, RELEASE);
            set_motor(enb, in3, in4, 0, RELEASE);
            break;
    }
}
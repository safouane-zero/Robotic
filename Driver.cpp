#include "header.hpp"


void configure_motor_pins(int en_pin, int inA_pin, int inB_pin){
    pinMode(en_pin, OUTPUT);
    pinMode(inA_pin, OUTPUT);
    pinMode(inB_pin, OUTPUT);
}


void set_motor(int en_pin, int inA_pin, int inB_pin, int speed, MotorDirection direction){
    analogWrite(en_pin, speed);

    switch (direction) {
        case FORWARD:
            digitalWrite(inA_pin, LOW);
            digitalWrite(inB_pin, HIGH);
            break;

        case BACKWARD:
            digitalWrite(inA_pin, HIGH);
            digitalWrite(inB_pin, LOW);
            break;

        case RELEASE:
        default: 
            digitalWrite(inA_pin, LOW);
            digitalWrite(inB_pin, LOW);
            break;
    }
}

void turn_motor(int ena, int in1, int in2, int enb, int in3, int in4, int speed, MotorDirection direction)
{
    switch (direction)
    {
    case LEFT:
        // Left motor goes BACKWARD, right motor goes FORWARD
        set_motor(ena, in1, in2, speed, BACKWARD);
        set_motor(enb, in3, in4, speed, FORWARD);
        break;
    
    case RIGHT:
        // Left motor goes FORWARD, right motor goes BACKWARD
        set_motor(ena, in1, in2, speed, FORWARD);
        set_motor(enb, in3, in4, speed, BACKWARD);
        break;

    default:
        // Stop both motors if any other direction is passed
        set_motor(ena, in1, in2, 0, RELEASE);
        set_motor(enb, in3, in4, 0, RELEASE);
        break;
    }
}


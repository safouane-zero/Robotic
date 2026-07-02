#include <Arduino.h>

// InfraRed Signals
const uint8_t BLACK = HIGH;
const uint8_t WHITE = LOW;
// Left Motor
const uint8_t ENA = 9;
const uint8_t IN1 = 8;
const uint8_t IN2 = 7;
// Right Motor
const uint8_t ENB = 3;
const uint8_t IN3 = 5;
const uint8_t IN4 = 4;

// InfraRed
const uint8_t R9 = 10;
const uint8_t R10 = 11;
const uint8_t R11 = 12;
const uint8_t R12 = 13;
const uint8_t LEFT_IR = 2;
const uint8_t RIGHT_IR = 6;

// LDR
const uint8_t ANALOG = A3;
//===============================================
inline void configure_pins() {
    // Motor Pins
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    // InfraRed Pins
    pinMode(R9, INPUT);
    pinMode(R10, INPUT);
    pinMode(R11, INPUT);
    pinMode(R12, INPUT);
    pinMode(LEFT_IR, INPUT);
    pinMode(RIGHT_IR, INPUT);
    // LDR Pins
    pinMode(ANALOG, INPUT);
}

inline void wait_for_start_signal(uint8_t pin) {
    while (analogRead(pin) >= 300) {}
}

inline void set_motor_forward(uint8_t en, uint8_t ina, uint8_t inb, uint8_t speed){
    analogWrite(en, speed);
    digitalWrite(ina, LOW);
    digitalWrite(inb, HIGH);
}

inline void set_motor_backward(uint8_t en, uint8_t ina, uint8_t inb, uint8_t speed){
    analogWrite(en, speed);
    digitalWrite(ina, HIGH);
    digitalWrite(inb, LOW);
}

inline void set_motor_stop(uint8_t en, uint8_t ina, uint8_t inb){
    analogWrite(en, 0);
    digitalWrite(ina, LOW);
    digitalWrite(inb, LOW);
}

inline uint8_t get_soft_slow_speed(uint8_t speed) {
    int val = (speed * 205) >> 8;
    return (val > 255) ? 255 : (val < 0 ? 0 : val);
}

inline uint8_t get_soft_fast_speed(uint8_t speed) {
    int val = (speed * 307) >> 8;
    return (val > 255) ? 255 : (val < 0 ? 0 : val);
}

inline void move_forward(uint8_t speed) {
    set_motor_forward(ENA, IN1, IN2, speed);
    set_motor_forward(ENB, IN3, IN4, speed);
}

inline void move_backward(uint8_t speed) {
    set_motor_backward(ENA, IN1, IN2, speed);
    set_motor_backward(ENB, IN3, IN4, speed);
}

inline void turn_left(uint8_t speed) {
    set_motor_backward(ENA, IN1, IN2, speed);
    set_motor_forward(ENB, IN3, IN4, speed);
}

inline void turn_right(uint8_t speed) {
    set_motor_forward(ENA, IN1, IN2, speed);
    set_motor_backward(ENB, IN3, IN4, speed);
}

inline void turn_soft_left(uint8_t speed) {
    set_motor_forward(ENA, IN1, IN2, get_soft_slow_speed(speed));
    set_motor_forward(ENB, IN3, IN4, get_soft_fast_speed(speed));
}

inline void turn_soft_right(uint8_t speed) {
    set_motor_forward(ENA, IN1, IN2, get_soft_fast_speed(speed));
    set_motor_forward(ENB, IN3, IN4, get_soft_slow_speed(speed));
}

inline void stop_robot() {
    set_motor_stop(ENA, IN1, IN2);
    set_motor_stop(ENB, IN3, IN4);
}
inline uint8_t read_ir_pattern() {
    uint8_t pattern = 0;

    pattern |= (digitalRead(LEFT_IR)  == BLACK) << 5;
    pattern |= (digitalRead(R12)       == BLACK) << 4;
    pattern |= (digitalRead(R11)      == BLACK) << 3;
    pattern |= (digitalRead(R10)      == BLACK) << 2;
    pattern |= (digitalRead(R9)      == BLACK) << 1;
    pattern |= (digitalRead(RIGHT_IR) == BLACK) << 0;

    return pattern;
}

// ==============================================
// ====================MAIN======================
// ==============================================

void setup(){
    configure_pins();
}

void loop()
{
}
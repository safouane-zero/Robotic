#include <Arduino.h>
#include <Ultrasonic.h>

// Left Motor (ENA=9, IN1=8, IN2=7)
// Right Motor (ENB=3, IN3=5, IN4=4)
// InfraRed (R9=10, R10=11, R12=12, LEFT_IR=2, RIGHT_IR=6)

const uint8_t ULTRA_SONIC_ECHO = A0;
const uint8_t ULTRA_SONIC_TRIG = 13;

Ultrasonic ultrasonic(ULTRA_SONIC_TRIG, ULTRA_SONIC_ECHO);

enum Move : uint8_t { NONE = 0, FORWARD, BACKWARD, LEFT, RIGHT, HARD_LEFT, HARD_RIGHT, STOP, ULTRA };

struct IRCase {
    Move    move;
    uint8_t speed;
};

IRCase table[32] = {};

void init_table() {
    table[0b00100] = {FORWARD, 180};
    table[0b10000] = {HARD_LEFT, 130};
    table[0b00001] = {HARD_RIGHT, 130};
    table[0b01000] = {LEFT, 130};
    table[0b00010] = {RIGHT, 130};
    table[0b11000] = {LEFT, 130};
    table[0b00011] = {RIGHT, 130};
    table[0b11100] = {LEFT, 130};
    table[0b00111] = {RIGHT, 130};
    table[0b11111] = {STOP, 0};
    table[0b00000] = {ULTRA, 0};
}

// ------------------------------------------------------------
// HIGH PERFORMANCE DIRECT PORT MANIPULATION FUNCTIONS
// ------------------------------------------------------------

inline __attribute__((always_inline)) uint8_t read_ir_pattern() {
    // Reads directly from hardware registers PINB and PIND
    // PD2(Left), PB4(R12), PB3(R10), PB2(R9), PD6(Right)
    return (((PIND >> 2) & 1) << 4) | 
           (((PINB >> 4) & 1) << 3) | 
           (((PINB >> 3) & 1) << 2) | 
           (((PINB >> 2) & 1) << 1) | 
           (((PIND >> 6) & 1) << 0);
}

inline __attribute__((always_inline)) void move_forward(uint8_t speed) {
    PORTB &= ~(1 << 0); PORTD |= (1 << 7);  // IN1 LOW, IN2 HIGH
    PORTD &= ~(1 << 5); PORTD |= (1 << 4);  // IN3 LOW, IN4 HIGH
    analogWrite(9, speed); analogWrite(3, speed);
}

inline __attribute__((always_inline)) void move_backward(uint8_t speed) {
    PORTB |= (1 << 0); PORTD &= ~(1 << 7);  // IN1 HIGH, IN2 LOW
    PORTD |= (1 << 5); PORTD &= ~(1 << 4);  // IN3 HIGH, IN4 LOW
    analogWrite(9, speed); analogWrite(3, speed);
}

inline __attribute__((always_inline)) void turn_left(uint8_t speed) {
    PORTB &= ~(1 << 0); PORTD &= ~(1 << 7); // IN1 LOW, IN2 LOW (Stop)
    PORTD &= ~(1 << 5); PORTD |= (1 << 4);  // IN3 LOW, IN4 HIGH
    analogWrite(9, 0); analogWrite(3, speed);
}

inline __attribute__((always_inline)) void turn_right(uint8_t speed) {
    PORTB &= ~(1 << 0); PORTD |= (1 << 7);  // IN1 LOW, IN2 HIGH
    PORTD &= ~(1 << 5); PORTD &= ~(1 << 4); // IN3 LOW, IN4 LOW (Stop)
    analogWrite(9, speed); analogWrite(3, 0);
}

inline __attribute__((always_inline)) void turn_hard_left(uint8_t speed) {
    PORTB |= (1 << 0); PORTD &= ~(1 << 7);  // IN1 HIGH, IN2 LOW
    PORTD &= ~(1 << 5); PORTD |= (1 << 4);  // IN3 LOW, IN4 HIGH
    analogWrite(9, speed); analogWrite(3, speed);
}

inline __attribute__((always_inline)) void turn_hard_right(uint8_t speed) {
    PORTB &= ~(1 << 0); PORTD |= (1 << 7);  // IN1 LOW, IN2 HIGH
    PORTD |= (1 << 5); PORTD &= ~(1 << 4);  // IN3 HIGH, IN4 LOW
    analogWrite(9, speed); analogWrite(3, speed);
}

inline __attribute__((always_inline)) void stop_robot() {
    PORTB &= ~(1 << 0); PORTD &= ~(1 << 7);
    PORTD &= ~(1 << 5); PORTD &= ~(1 << 4);
    analogWrite(9, 0); analogWrite(3, 0);
}

inline __attribute__((always_inline)) void execute_move(Move move, uint8_t speed) {
    switch (move) {
        case FORWARD:    move_forward(speed);    break;
        case BACKWARD:   move_backward(speed);   break;
        case LEFT:       turn_left(speed);       break;
        case RIGHT:      turn_right(speed);      break;
        case STOP:       stop_robot();           break;
        case HARD_LEFT:  turn_hard_left(speed);  break;
        case HARD_RIGHT: turn_hard_right(speed); break;
        default: break; 
    }
}

inline __attribute__((always_inline)) uint16_t read_ldr() {
    ADMUX = (1 << REFS0) | 3;     // Set reference to AVCC and select channel ADC3 (A3)
    ADCSRA |= (1 << ADSC);        // Start ADC conversion
    while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete (hardware clears this bit)
    return ADC;                   // Return the 10-bit raw value (0 - 1023)
}
// ------------------------------------------------------------
// MAIN
// ------------------------------------------------------------

unsigned long start = 0;

void setup() {
    DDRB |= (1 << 0) | (1 << 1);
    DDRD |= (1 << 3) | (1 << 4) | (1 << 5) | (1 << 7);
    
    DDRB &= ~((1 << 2) | (1 << 3) | (1 << 4));
    DDRD &= ~((1 << 2) | (1 << 6));

    pinMode(A3, INPUT);
    pinMode(ULTRA_SONIC_ECHO, INPUT);
    pinMode(ULTRA_SONIC_TRIG, OUTPUT);


    while (read_ldr() >= 90);

    init_table();
    start = millis();
}

void loop() {
    uint8_t ir = read_ir_pattern();
    IRCase c = table[ir];

    if ((millis() - start) >= 25000 && ir == 0b11111){
        move_forward(200);
        delay(500);
        stop_robot();
        delay(10000);
    }
    else if (c.move == ULTRA) {
        uint16_t ultrasonic_distance = ultrasonic.read(); 
        
        if (ultrasonic_distance < 12) {
            if (ultrasonic_distance >= 6 && ultrasonic_distance <= 9) move_forward(120);
            else if (ultrasonic_distance > 9) turn_left(100);
            else turn_right(100);
        }
        else if (millis() - start >= 13000 && millis() - start <= 17000) {
            turn_hard_right(100);
        }
    }
    else if (c.move != NONE) {
        execute_move(c.move, c.speed);
    }
}

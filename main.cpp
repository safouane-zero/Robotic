#include <Arduino.h>

// ============================================================
// PIN MAP
// ============================================================

// InfraRed Signals
const bool BLACK = true;
const bool WHITE = false;

// Left Motor
const uint8_t ENA = 9;
const uint8_t IN1 = 8;
const uint8_t IN2 = 7;
// Right Motor
const uint8_t ENB = 3;
const uint8_t IN3 = 5;
const uint8_t IN4 = 4;

// InfraRed sensors
const uint8_t R9  = 10;
const uint8_t R10 = 11;
const uint8_t R11 = 12;
const uint8_t R12 = 13;
const uint8_t LEFT_IR  = 2;
const uint8_t RIGHT_IR = 6;

// LDR
const uint8_t ANALOG = A3;

// ============================================================
// TYPES
// ============================================================
// MOVE is just a label for "what kind of motion". NONE = 0, which
// is what every non-listed table slot gets automatically, so any
// pattern you don't set stays NONE by default.

enum Move : uint8_t {
    NONE = 0,   // undefined -> loop() does nothing -> robot keeps last move
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    SOFT_LEFT,
    SOFT_RIGHT,
    STOP
};

struct IRCase {
    Move    move;
    uint8_t speed;
};

// ============================================================
// FUNCTION PROTOTYPES
// ============================================================
// .ino files get these auto-generated for you by the Arduino IDE.
// A plain .cpp file doesn't, so they're declared explicitly here.

void configure_pins();
void set_motor_forward(uint8_t en, uint8_t ina, uint8_t inb, uint8_t speed);
void set_motor_backward(uint8_t en, uint8_t ina, uint8_t inb, uint8_t speed);
void set_motor_stop(uint8_t en, uint8_t ina, uint8_t inb);
void move_forward(uint8_t speed);
void move_backward(uint8_t speed);
void turn_left(uint8_t speed);
void turn_right(uint8_t speed);
void turn_soft_left(uint8_t speed);
void turn_soft_right(uint8_t speed);
void stop_robot();
uint8_t read_ir_pattern();
void execute_move(Move move, uint8_t speed);
void init_table();
void setup();
void loop();

// ============================================================
// THE ARRAY
// ============================================================
// Bit layout of the IR pattern used as the index (bit5 = leftmost
// sensor ... bit0 = rightmost sensor):
//
//   LEFT_IR   R12   R11   R10   R9   RIGHT_IR
//    bit5     bit4  bit3  bit2  bit1   bit0
//
// 6 sensors -> 2^6 = 64 possible patterns (0x00 to 0x3F).
//
// This IS the lookup table: index = 6-bit IR pattern, value = {move, speed}.
// It starts zero-initialized, meaning every slot is {NONE, 0} by default.
// init_table() below fills in only the patterns you actually care about.
//
// (Note: this is filled with plain assignments rather than a single
// initializer list because the AVR-GCC toolchain used for Uno doesn't
// support designated initializers for array slots holding a struct.)

IRCase table[64] = {};

void init_table() {
    table[0b00000] = {FORWARD, 190};
    table[0b00100] = {FORWARD, 255};
    table[0b01000] = {LEFT, 100};
    table[0b00010] = {RIGHT,100};
    table[0b10000] = {LEFT, 200};
    table[0b00001] = {RIGHT, 200};
    table[0b11111] = {STOP, 0};

}

// ============================================================
// MAIN
// ============================================================
void print_ir_pattern(uint8_t pattern) {
    Serial.print("IR Pattern: ");
    // Loop through the 6 bits from left (bit 5) to right (bit 0)
    for (int i = 5; i >= 0; i--) {
        Serial.print((pattern >> i) & 1);
    }
    Serial.println(); // Add a new line
}

void setup() {
    Serial.begin(9600);
    configure_pins();
    init_table();
    delay(3000);
}

void loop() {
    uint8_t ir = read_ir_pattern();
    IRCase c = table[ir];

    if (c.move != NONE) {
        execute_move(c.move, c.speed);
    }
    if (c.move == 1)
        Serial.println("FORWARD");
    else if (c.move == 3)
        Serial.println("LEFT");
    else if (c.move == 4)
        Serial.println("RIGHT");
    else if (c.move == 5)
        Serial.println("SOFT_LEFT");
    else if (c.move == 6)
        Serial.println("SOFT_RIGHT");
    else if (c.move == 7)
        Serial.println("STOP");
    else 
        Serial.println("NONE");

}

// ============================================================
// IMPLEMENTATION
// ============================================================

void configure_pins() {
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    pinMode(R9, INPUT);
    pinMode(R10, INPUT);
    pinMode(R11, INPUT);
    pinMode(R12, INPUT);
    pinMode(LEFT_IR, INPUT);
    pinMode(RIGHT_IR, INPUT);

    pinMode(ANALOG, INPUT);
}

void set_motor_forward(uint8_t en, uint8_t ina, uint8_t inb, uint8_t speed) {
    analogWrite(en, speed);
    digitalWrite(ina, LOW);
    digitalWrite(inb, HIGH);
}

void set_motor_backward(uint8_t en, uint8_t ina, uint8_t inb, uint8_t speed) {
    analogWrite(en, speed);
    digitalWrite(ina, HIGH);
    digitalWrite(inb, LOW);
}

void set_motor_stop(uint8_t en, uint8_t ina, uint8_t inb) {
    analogWrite(en, 0);
    digitalWrite(ina, LOW);
    digitalWrite(inb, LOW);
}

void move_forward(uint8_t speed) {
    set_motor_forward(ENA, IN1, IN2, speed);
    set_motor_forward(ENB, IN3, IN4, speed);
}

void move_backward(uint8_t speed) {
    set_motor_backward(ENA, IN1, IN2, speed);
    set_motor_backward(ENB, IN3, IN4, speed);
}

void turn_left(uint8_t speed) {
    // set_motor_backward(ENA, IN1, IN2, speed);
    set_motor_stop(ENA, IN1, IN2);
    set_motor_forward(ENB, IN3, IN4, speed);
}

void turn_right(uint8_t speed) {
    set_motor_forward(ENA, IN1, IN2, speed);
    // set_motor_backward(ENB, IN3, IN4, speed);
    set_motor_stop(ENB, IN3, IN4);
}

void turn_soft_left(uint8_t speed) {
    uint16_t slow = ((uint16_t)speed * 205) >> 8;
    uint16_t fast = ((uint16_t)speed * 307) >> 8;
    if (fast > 255) fast = 255;
    set_motor_forward(ENA, IN1, IN2, (uint8_t)slow);
    set_motor_forward(ENB, IN3, IN4, (uint8_t)fast);
}

void turn_soft_right(uint8_t speed) {
    uint16_t slow = ((uint16_t)speed * 205) >> 8;
    uint16_t fast = ((uint16_t)speed * 307) >> 8;
    if (fast > 255) fast = 255;
    set_motor_forward(ENA, IN1, IN2, (uint8_t)fast);
    set_motor_forward(ENB, IN3, IN4, (uint8_t)slow);
}

void stop_robot() {
    set_motor_stop(ENA, IN1, IN2);
    set_motor_stop(ENB, IN3, IN4);
}

uint8_t read_ir_pattern() {
    uint8_t pattern = 0;
    pattern |= (digitalRead(LEFT_IR)  == BLACK) << 4;
    pattern |= (digitalRead(R12)      == BLACK) << 3;
    pattern |= (digitalRead(R10)      == BLACK) << 2;
    pattern |= (digitalRead(R9)       == BLACK) << 1;
    pattern |= (digitalRead(RIGHT_IR) == BLACK) << 0;
    return pattern;
}

void execute_move(Move move, uint8_t speed) {
    switch (move) {
        case FORWARD:    move_forward(speed);    break;
        case BACKWARD:   move_backward(speed);   break;
        case LEFT:       turn_left(speed);       break;
        case RIGHT:      turn_right(speed);      break;
        case SOFT_LEFT:  turn_soft_left(speed);  break;
        case SOFT_RIGHT: turn_soft_right(speed); break;
        case STOP:       stop_robot();           break;
        case NONE:       break; // nothing to do
    }
}

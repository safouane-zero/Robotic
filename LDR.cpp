#include "header.hpp"

void configure_LDR(uint8_t pin) {
    pinMode(pin, INPUT);
}

int readLDR(uint8_t pin) {
    return analogRead(pin);
}
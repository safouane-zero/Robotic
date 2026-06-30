#include "header.hpp"

// LDR ==========================================
int LDR(uint8_t pin){
    return analogRead(pin);
}

void configure_LDR(uint8_t pin){
    pinMode(pin, INPUT);
}
// ==============================================
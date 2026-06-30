#include "header.hpp"
// 4-Channel IR Sensor — Obstacle Avoidance
// CH1 = Front-Left, CH2 = Front-Right
// CH3 = Rear-Left,  CH4 = Rear-Right


const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;

void setup() {
  pinMode(IN1, INPUT);
  pinMode(IN2, INPUT);
  pinMode(IN3, INPUT);
  pinMode(IN4, INPUT);
  Serial.begin(9600);
}

void loop() {
    
    int R9 = digitalRead(IN1);
    int R10 = digitalRead(IN2);
    int R11 = digitalRead(IN3);
    int R12 = digitalRead(IN4);

    Serial.print("R9: ");
    Serial.print(R9);
    Serial.print(" R10: ");
    Serial.print(R10);
    Serial.print(" R11: ");
    Serial.print(R11);
    Serial.print(" R12: ");
    Serial.print(R12);
    Serial.println("");
    delay(500);
}
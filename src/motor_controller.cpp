#include "motor_controller.h"




MotorController::MotorController() {
    // nur Variablen
}



void MotorController::begin() {
    pinMode(MOTOR_LEFT_IN1, OUTPUT);
    pinMode(MOTOR_LEFT_IN2, OUTPUT);
    pinMode(MOTOR_LEFT_ENA, OUTPUT);
    pinMode(MOTOR_RIGHT_IN3, OUTPUT);
    pinMode(MOTOR_RIGHT_IN4, OUTPUT);
    pinMode(MOTOR_RIGHT_ENB, OUTPUT);
    stop();
}

void MotorController::setLeft(int speed) {
    applyMotor(MOTOR_LEFT_ENA, MOTOR_LEFT_IN1, MOTOR_LEFT_IN2, speed);
}

void MotorController::setRight(int speed) {
    applyMotor(MOTOR_RIGHT_ENB, MOTOR_RIGHT_IN3, MOTOR_RIGHT_IN4, speed);
}

void MotorController::drive(int left, int right) {
    setLeft(left);
    setRight(right);
}

void MotorController::stop() {
    drive(0, 0);
}

void MotorController::applyMotor(uint8_t pinEn, uint8_t pinA, uint8_t pinB, int speed) {
    if (speed > 0) {
        digitalWrite(pinA, HIGH);
        digitalWrite(pinB, LOW);
    } else if (speed < 0) {
        digitalWrite(pinA, LOW);
        digitalWrite(pinB, HIGH);
        speed = -speed;
    } else {
        digitalWrite(pinA, LOW);
        digitalWrite(pinB, LOW);
    }
    analogWrite(pinEn, constrain(speed, 0, 255));
}

// motor_controller.cpp — Motorsteuerung über L298N
// Positive Werte = vorwärts, negative Werte = rückwärts, 0 = Stop.
// Wertebereich: -255 bis +255 (entspricht 8-Bit PWM)

#include "motor_controller.h"

void MotorController::begin() {
    // Alle Motor-Pins als Ausgang konfigurieren
    pinMode(MOTOR_LEFT_IN1,  OUTPUT);
    pinMode(MOTOR_LEFT_IN2,  OUTPUT);
    pinMode(MOTOR_LEFT_ENA,  OUTPUT);
    pinMode(MOTOR_RIGHT_IN3, OUTPUT);
    pinMode(MOTOR_RIGHT_IN4, OUTPUT);
    pinMode(MOTOR_RIGHT_ENB, OUTPUT);

    // Sicherheitshalber sofort stoppen
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
        // Vorwärts: IN1=HIGH, IN2=LOW
        digitalWrite(pinA, HIGH);
        digitalWrite(pinB, LOW);
    } else if (speed < 0) {
        // Rückwärts: IN1=LOW, IN2=HIGH — Geschwindigkeit ist immer positiv für PWM
        digitalWrite(pinA, LOW);
        digitalWrite(pinB, HIGH);
        speed = -speed;
    } else {
        // Stop: beide Richtungs-Pins LOW → Motor wird kurzgeschlossen (Bremse)
        digitalWrite(pinA, LOW);
        digitalWrite(pinB, LOW);
    }

    // PWM-Signal an Enable-Pin — bestimmt die Geschwindigkeit (0–255)
    analogWrite(pinEn, constrain(speed, 0, 255));
}

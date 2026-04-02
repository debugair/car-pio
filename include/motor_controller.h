// motor_controller.h — Schnittstelle für die Motorsteuerung
// Steuert zwei DC-Motoren über einen L298N Motortreiber per PWM.
#pragma once
#include <Arduino.h>
#include "config.h"

class MotorController {
public:
    MotorController() = default;

    void begin();                    // GPIO-Pins initialisieren, Motoren stoppen
    void setLeft(int speed);         // Linken Motor setzen  (-255 bis +255)
    void setRight(int speed);        // Rechten Motor setzen (-255 bis +255)
    void drive(int left, int right); // Beide Motoren gleichzeitig setzen
    void stop();                     // Beide Motoren sofort stoppen

private:
    // Interne Hilfsfunktion: setzt Richtungs-Pins und PWM für einen Motor
    // pinEn = Enable/PWM-Pin, pinA/pinB = Richtungs-Pins, speed = -255..255
    void applyMotor(uint8_t pinEn, uint8_t pinA, uint8_t pinB, int speed);
};

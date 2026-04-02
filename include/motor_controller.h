#pragma once
#include <Arduino.h>
#include "config.h"

class MotorController {
public:
    MotorController() ;        // deklariert 2026-04-01 20:15  falsch: MotorController::MotorController() ; 
    void begin();
    void setLeft(int speed);
    void setRight(int speed);
    void drive(int left, int right);
    void stop();

private:
    void applyMotor(uint8_t pinEn, uint8_t pinA, uint8_t pinB, int speed);
};

#pragma once
#include <Arduino.h>
#include "config.h"

class EncoderMonitor {
public:
    EncoderMonitor() = default;
    void begin();
    void update();
    float getLeftRPM() const { return _leftRPM; }
    float getRightRPM() const { return _rightRPM; }

private:
    volatile unsigned long _leftPulses = 0;
    volatile unsigned long _rightPulses = 0;
    unsigned long _lastUpdate = 0;
    float _leftRPM = 0;
    float _rightRPM = 0;

    static EncoderMonitor* _instance;
    static void IRAM_ATTR leftISR();
    static void IRAM_ATTR rightISR();
};





/*
#pragma once
#include <Arduino.h>
#include "config.h"

class EncoderMonitor {
public:
    EncoderMonitor();

    void begin();
    void update();

    float getLeftRPM() const;
    float getRightRPM() const;

private:
    // Pulszähler (ISR schreibt, loop liest)
    volatile uint32_t _leftPulses;
    volatile uint32_t _rightPulses;

    // Timing / Ergebnis
    uint32_t _lastUpdate;
    float _leftRPM;
    float _rightRPM;

    // ESP32 Critical Section
    portMUX_TYPE _mux;

    // ISR Zugriff
    static EncoderMonitor* _instance;
    static void IRAM_ATTR leftISR();
    static void IRAM_ATTR rightISR();
};



#pragma once
#include <Arduino.h>
#include "config.h"

class EncoderMonitor {
public:
    void begin();
    void update();  // call periodically to compute RPM

    float getLeftRPM() const;
    float getRightRPM() const;

private:
    volatile unsigned long _leftPulses = 0;
    volatile unsigned long _rightPulses = 0;
    unsigned long _lastUpdate = 0;
    float _leftRPM = 0;
    float _rightRPM = 0;

    static EncoderMonitor* _instance;
    static void IRAM_ATTR leftISR();
    static void IRAM_ATTR rightISR();
};
*/

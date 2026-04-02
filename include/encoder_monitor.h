// encoder_monitor.h — Schnittstelle für Raddrehzahl-Messung
#pragma once
#include <Arduino.h>
#include "config.h"

class EncoderMonitor {
public:
    EncoderMonitor() = default;

    void  begin();  // Pins und Interrupts initialisieren
    void  update(); // RPM berechnen — muss regelmäßig in loop() aufgerufen werden

    float getLeftRPM()  const { return _leftRPM; }
    float getRightRPM() const { return _rightRPM; }

private:
    // Pulse-Zähler — volatile weil ISR und loop() gleichzeitig darauf zugreifen können
    volatile uint32_t _leftPulses  = 0;
    volatile uint32_t _rightPulses = 0;

    unsigned long _lastUpdate = 0;  // Zeitstempel der letzten RPM-Berechnung
    float _leftRPM  = 0;
    float _rightRPM = 0;

    // Mutex für sicheren Zugriff zwischen ISR und Hauptprogramm (ESP32 dual-core!)
    portMUX_TYPE _mux = portMUX_INITIALIZER_UNLOCKED;

    // Singleton — notwendig weil attachInterrupt() nur statische Funktionen erlaubt
    static EncoderMonitor* _instance;

    static void IRAM_ATTR leftISR();   // Interrupt-Handler linkes Rad  (IRAM = schnell!)
    static void IRAM_ATTR rightISR();  // Interrupt-Handler rechtes Rad
};

// encoder_monitor.cpp — Raddrehzahl-Messung per Hardware-Interrupt
// Zählt Encoder-Pulse beider Räder und berechnet daraus die Drehzahl in RPM.

#include "encoder_monitor.h"

// Singleton-Instanz für den statischen ISR-Zugriff
EncoderMonitor* EncoderMonitor::_instance = nullptr;

void EncoderMonitor::begin() {
    _instance = this;

    // Encoder-Pins als Eingang mit internem Pull-up konfigurieren
    pinMode(ENCODER_LEFT,  INPUT_PULLUP);
    pinMode(ENCODER_RIGHT, INPUT_PULLUP);

    // Bei jeder steigenden Flanke (Schlitz passiert Lichtschranke) → ISR aufrufen
    attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT),  leftISR,  RISING);
    attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT), rightISR, RISING);

    _lastUpdate = millis();
}

void EncoderMonitor::update() {
    unsigned long now     = millis();
    unsigned long elapsed = now - _lastUpdate;

    // Nur alle TELEMETRY_INTERVAL_MS auswerten, nicht bei jedem loop()-Aufruf
    if (elapsed < TELEMETRY_INTERVAL_MS) return;

    // Pulse atomar auslesen und Zähler zurücksetzen.
    // portENTER_CRITICAL verhindert, dass der ISR während des Auslesens unterbricht
    // und dabei einen Pulse verloren geht oder doppelt gezählt wird.
    uint32_t lp, rp;
    portENTER_CRITICAL(&_mux);
    lp = _leftPulses;  _leftPulses  = 0;
    rp = _rightPulses; _rightPulses = 0;
    portEXIT_CRITICAL(&_mux);

    // RPM berechnen: (Pulse / Schlitze pro Umdrehung) / Sekunden × 60
    float seconds = elapsed / 1000.0f;
    float rawLeft  = (lp / (float)ENCODER_SLOTS) / seconds * 60.0f;
    float rawRight = (rp / (float)ENCODER_SLOTS) / seconds * 60.0f;

    // Exponentieller gleitender Mittelwert — glättet Rauschen einzelner Messzyklen
    // 0.3 = 30% neuer Wert, 70% alter Wert
    const float alpha = 0.3f;
    _leftRPM  = alpha * rawLeft  + (1.0f - alpha) * _leftRPM;
    _rightRPM = alpha * rawRight + (1.0f - alpha) * _rightRPM;

    _lastUpdate = now;
}

// ISR für linkes Rad — wird bei jeder Encoder-Flanke aufgerufen (sehr kurz halten!)
void IRAM_ATTR EncoderMonitor::leftISR() {
    if (_instance) {
        portENTER_CRITICAL_ISR(&_instance->_mux);
        _instance->_leftPulses++;
        portEXIT_CRITICAL_ISR(&_instance->_mux);
    }
}

// ISR für rechtes Rad
void IRAM_ATTR EncoderMonitor::rightISR() {
    if (_instance) {
        portENTER_CRITICAL_ISR(&_instance->_mux);
        _instance->_rightPulses++;
        portEXIT_CRITICAL_ISR(&_instance->_mux);
    }
}

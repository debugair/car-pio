
#include "encoder_monitor.h"

EncoderMonitor* EncoderMonitor::_instance = nullptr;

void EncoderMonitor::begin() {
    _instance = this;

    pinMode(ENCODER_LEFT, INPUT_PULLUP);
    pinMode(ENCODER_RIGHT, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT), leftISR, RISING);
    attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT), rightISR, RISING);

    _lastUpdate = millis();
}

void EncoderMonitor::update() {
    unsigned long now = millis();
    unsigned long elapsed = now - _lastUpdate;
    if (elapsed < TELEMETRY_INTERVAL_MS) return;

    noInterrupts();
    unsigned long lp = _leftPulses;
    unsigned long rp = _rightPulses;
    _leftPulses = 0;
    _rightPulses = 0;
    interrupts();

    float seconds = elapsed / 1000.0f;
    _leftRPM = (lp / (float)ENCODER_SLOTS) / seconds * 60.0f;
    _rightRPM = (rp / (float)ENCODER_SLOTS) / seconds * 60.0f;

    _lastUpdate = now;
}

void IRAM_ATTR EncoderMonitor::leftISR() {
    if (_instance) _instance->_leftPulses++;
}

void IRAM_ATTR EncoderMonitor::rightISR() {
    if (_instance) _instance->_rightPulses++;
}



/*#include "encoder_monitor.h"

EncoderMonitor* EncoderMonitor::_instance = nullptr;

EncoderMonitor::EncoderMonitor()
    : _leftPulses(0),
      _rightPulses(0),
      _lastUpdate(0),
      _leftRPM(0.0f),
      _rightRPM(0.0f),
      _mux(portMUX_INITIALIZER_UNLOCKED) {
    // ❗ KEINE Hardware hier
}

void EncoderMonitor::begin() {
    _instance = this;

    pinMode(ENCODER_LEFT, INPUT_PULLUP);
    pinMode(ENCODER_RIGHT, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT), leftISR, RISING);
    attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT), rightISR, RISING);

    _lastUpdate = millis();
}

void EncoderMonitor::update() {
    uint32_t now = millis();
    uint32_t elapsed = now - _lastUpdate;

    if (elapsed < TELEMETRY_INTERVAL_MS) return;

    uint32_t lp, rp;

    // 🔒 saubere Synchronisation mit ISR
    portENTER_CRITICAL(&_mux);
    lp = _leftPulses;
    rp = _rightPulses;
    _leftPulses = 0;
    _rightPulses = 0;
    portEXIT_CRITICAL(&_mux);

    float seconds = elapsed * 0.001f;

    if (seconds > 0.0f) {
        _leftRPM  = (lp / (float)ENCODER_SLOTS) / seconds * 60.0f;
        _rightRPM = (rp / (float)ENCODER_SLOTS) / seconds * 60.0f;
    }

    _lastUpdate = now;
}

float EncoderMonitor::getLeftRPM() const {
    return _leftRPM;
}

float EncoderMonitor::getRightRPM() const {
    return _rightRPM;
}

void IRAM_ATTR EncoderMonitor::leftISR() {
    if (EncoderMonitor::_instance) {
        portENTER_CRITICAL_ISR(&EncoderMonitor::_instance->_mux);
        EncoderMonitor::_instance->_leftPulses++;
        portEXIT_CRITICAL_ISR(&EncoderMonitor::_instance->_mux);
    }
}

void IRAM_ATTR EncoderMonitor::rightISR() {
    if (EncoderMonitor::_instance) {
        portENTER_CRITICAL_ISR(&EncoderMonitor::_instance->_mux);
        EncoderMonitor::_instance->_rightPulses++;
        portEXIT_CRITICAL_ISR(&EncoderMonitor::_instance->_mux);
    }
}




 #include "encoder_monitor.h"

EncoderMonitor* EncoderMonitor::_instance = nullptr;

void EncoderMonitor::begin() {
    _instance = this;

    pinMode(ENCODER_LEFT, INPUT_PULLUP);
    pinMode(ENCODER_RIGHT, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT), leftISR, RISING);
    attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT), rightISR, RISING);

    _lastUpdate = millis();
}

void EncoderMonitor::update() {
    unsigned long now = millis();
    unsigned long elapsed = now - _lastUpdate;
    if (elapsed < TELEMETRY_INTERVAL_MS) return;

    noInterrupts();
    unsigned long lp = _leftPulses;
    unsigned long rp = _rightPulses;
    _leftPulses = 0;
    _rightPulses = 0;
    interrupts();

    float seconds = elapsed / 1000.0f;
    _leftRPM = (lp / (float)ENCODER_SLOTS) / seconds * 60.0f;
    _rightRPM = (rp / (float)ENCODER_SLOTS) / seconds * 60.0f;
    _lastUpdate = now;
}

float EncoderMonitor::getLeftRPM() const { return _leftRPM; }
float EncoderMonitor::getRightRPM() const { return _rightRPM; }

void IRAM_ATTR EncoderMonitor::leftISR() {
    if (_instance) _instance->_leftPulses++;
}

void IRAM_ATTR EncoderMonitor::rightISR() {
    if (_instance) _instance->_rightPulses++;
}
 */
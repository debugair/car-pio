/* #include "battery_monitor.h"

void BatteryMonitor::begin() {
    pinMode(BATTERY_ADC_PIN, INPUT);
    analogReadResolution(12);  // 12-bit ADC (0–4095)
    // Take an initial reading
    update();
}

void BatteryMonitor::update() {
    if (millis() - _lastUpdate < TELEMETRY_INTERVAL_MS) return;
    _lastUpdate = millis();

    // Average multiple samples to reduce noise
    uint32_t sum = 0;
    for (int i = 0; i < BATTERY_SAMPLES; i++) {
        sum += analogRead(BATTERY_ADC_PIN);
    }
    float adcAvg = (float)sum / BATTERY_SAMPLES;

    // ESP32-S3 ADC: 12-bit (0–4095), 0–3.3V reference
    float adcVoltage = (adcAvg / 4095.0) * 3.3;
    _voltage = adcVoltage * BATTERY_DIVIDER_RATIO;
}

float BatteryMonitor::getVoltage() const {
    return _voltage;
}

int BatteryMonitor::getPercent() const {
    float pct = (_voltage - BATTERY_LOW_VOLTAGE) / (BATTERY_FULL_VOLTAGE - BATTERY_LOW_VOLTAGE) * 100.0;
    if (pct < 0) pct = 0;
    if (pct > 100) pct = 100;
    return (int)pct;
}

bool BatteryMonitor::isLow() const {
    return _voltage > 0.5 && _voltage < BATTERY_LOW_VOLTAGE;
}
 */
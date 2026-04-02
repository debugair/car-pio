#include "comms_manager.h"

CommsManager* CommsManager::_instance = nullptr;

CommsManager::CommsManager(MotorController& motors, EncoderMonitor& encoders)
    : _motors(motors), _encoders(encoders),
      _ws(nullptr), _lastCommandTime(0), _lastTelemetry(0)
{
    _instance = this;
}

void CommsManager::begin() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("\nConnected! IP: %s\n", WiFi.localIP().toString().c_str());

    _ws = new WebSocketsServer(WEBSOCKET_PORT);
    _ws->begin();
    _ws->onEvent(eventTrampoline);
}

void CommsManager::loop() {
    if (_ws) _ws->loop();
    sendTelemetry();
}

bool CommsManager::isCommandTimedOut() const {
    return millis() - _lastCommandTime > DEADMAN_TIMEOUT_MS;
}

void CommsManager::sendTelemetry() {
    if (millis() - _lastTelemetry < TELEMETRY_INTERVAL_MS) return;
    _lastTelemetry = millis();

    char buf[64];
    snprintf(buf, sizeof(buf), "T:%.1f,%.1f", _encoders.getLeftRPM(), _encoders.getRightRPM());
    if (_ws) _ws->broadcastTXT(buf);
}

// void CommsManager::onEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
//     // Dummy
//  ersetzt 2026-04-02
// }
void CommsManager::onEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {

    switch(type) {

        case WStype_CONNECTED: {
            IPAddress ip = _ws->remoteIP(num);
            Serial.printf("Client %u connected from %s\n",
                          num, ip.toString().c_str());
            break;
        }

        case WStype_DISCONNECTED:
            Serial.printf("Client %u disconnected\n", num);
            break;

        case WStype_TEXT: {
            // payload[length] = 0;

            // Serial.printf("Received: %s\n", (char*)payload);
            char msg[64];
            size_t len = min(length, sizeof(msg)-1);
            memcpy(msg, payload, len);
            msg[len] = 0;

            Serial.printf("Received: %s\n", msg);

            int left = 0;
            int right = 0;

            if (sscanf((char*)payload, "%d,%d", &left, &right) == 2) {

                //_motors.setSpeed(left, right);
                _motors.setLeft(left);
                _motors.setRight(right);

                _lastCommandTime = millis();

                Serial.printf("Motor L:%d R:%d\n", left, right);
            }
            break;
        }

        default:
            break;
    }
}
void CommsManager::eventTrampoline(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    if (_instance) _instance->onEvent(num, type, payload, length);
}

/*

#include "comms_manager.h"
#include "motor_controller.h"
#include "encoder_monitor.h"
#include "battery_monitor.h"
#include "config.h"
#include <WiFi.h>





CommsManager* CommsManager::_instance = nullptr;

CommsManager::CommsManager(MotorController& motors,
                           EncoderMonitor& encoders,
                           BatteryMonitor& battery)
    : _motors(motors),
      _encoders(encoders),
      _battery(battery),
      _ws(nullptr),
      _lastCommandTime(0),
      _lastTelemetry(0) {
    _instance = this;
}

void CommsManager::begin() {
    Serial.println("[COMMS] begin()");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.printf("Connecting to %s", WIFI_SSID);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.printf("\nConnected! IP: %s\n", WiFi.localIP().toString().c_str());

    // ❗ WebSocket erst HIER erzeugen
    _ws = new WebSocketsServer(WEBSOCKET_PORT);
    _ws->begin();
    _ws->onEvent(eventTrampoline);

    Serial.printf("WebSocket server started on port %d\n", WEBSOCKET_PORT);
}

void CommsManager::loop() {
    if (_ws) {
        _ws->loop();
    }
    sendTelemetry();
}

bool CommsManager::isCommandTimedOut() const {
    return millis() - _lastCommandTime > DEADMAN_TIMEOUT_MS;
}



 #include "comms_manager.h"

CommsManager* CommsManager::_instance = nullptr;

CommsManager::CommsManager(MotorController& motors, EncoderMonitor& encoders, BatteryMonitor& battery)
    : _motors(motors), _encoders(encoders), _battery(battery), _ws(WEBSOCKET_PORT),
      _lastCommandTime(0), _lastTelemetry(0) {
    _instance = this;
}

void CommsManager::begin() {
    Serial.begin(115200);
    delay(1000);
#ifdef STATIC_IP
    IPAddress ip(STATIC_IP);
    IPAddress gateway(STATIC_GATEWAY);
    IPAddress subnet(STATIC_SUBNET);
    IPAddress dns(STATIC_DNS);
    WiFi.config(ip, gateway, subnet, dns);
    Serial.printf("Using static IP: %s\n", ip.toString().c_str());
#endif

    Serial.printf("Connecting to %s", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("\nConnected! IP: %s\n", WiFi.localIP().toString().c_str());

    _ws.begin();
    _ws.onEvent(eventTrampoline);
    Serial.printf("WebSocket server on ws://%s:%d\n", WiFi.localIP().toString().c_str(), WEBSOCKET_PORT);
}

void CommsManager::loop() {
    _ws.loop();
    sendTelemetry();
}

bool CommsManager::isCommandTimedOut() const {
    return millis() - _lastCommandTime > DEADMAN_TIMEOUT_MS;
}

void CommsManager::handleCommand(const char* payload) {
    int left = 0, right = 0;
    if (sscanf(payload, "%d,%d", &left, &right) == 2) {
        _motors.drive(left, right);
        _lastCommandTime = millis();
    }
}

void CommsManager::sendTelemetry() {
    if (millis() - _lastTelemetry < TELEMETRY_INTERVAL_MS) return;
    _lastTelemetry = millis();

    char buf[64];
    snprintf(buf, sizeof(buf), "T:%.1f,%.1f,%.2f",
        _encoders.getLeftRPM(), _encoders.getRightRPM(), _battery.getVoltage());
    _ws.broadcastTXT(buf);
}

void CommsManager::onEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_CONNECTED:
            Serial.printf("[WS] Client #%u connected\n", num);
            break;
        case WStype_DISCONNECTED:
            Serial.printf("[WS] Client #%u disconnected\n", num);
            _motors.stop();
            break;
        case WStype_TEXT:
            handleCommand((const char*)payload);
            break;
        default:
            break;
    }
}

void CommsManager::eventTrampoline(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    if (_instance) {
        _instance->onEvent(num, type, payload, length);
    }
}
 */
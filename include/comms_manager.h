
// comms_manager.h

#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include "motor_controller.h"
#include "encoder_monitor.h"
#include "config.h"

class CommsManager {
public:
    CommsManager(MotorController& motors, EncoderMonitor& encoders);
    void begin();
    void loop();
    bool isCommandTimedOut() const;

private:
    MotorController& _motors;
    EncoderMonitor& _encoders;
    WebSocketsServer* _ws;
    unsigned long _lastCommandTime;
    unsigned long _lastTelemetry;

    static CommsManager* _instance;
    static void eventTrampoline(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
    void onEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
    void sendTelemetry();
};




/* #pragma once

#include <Arduino.h>
#include <WebSocketsServer.h>

class MotorController;
class EncoderMonitor;
class BatteryMonitor;

class CommsManager {
public:
    CommsManager(MotorController& motors,
                 EncoderMonitor& encoders,
                 BatteryMonitor& battery);

    void begin();
    void loop();
    bool isCommandTimedOut() const;

private:
    MotorController& _motors;
    EncoderMonitor& _encoders;
    BatteryMonitor& _battery;

    WebSocketsServer* _ws;

    unsigned long _lastCommandTime;
    unsigned long _lastTelemetry;

    static CommsManager* _instance;

    static void eventTrampoline(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
    void onEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);

    void handleCommand(const char* payload);
    void sendTelemetry();
}; */








/* #include <WebSocketsServer.h>

class CommsManager {
public:
    CommsManager(MotorController& motors,
                 EncoderMonitor& encoders,
                 BatteryMonitor& battery);

    void begin();
    void loop();
    bool isCommandTimedOut() const;

private:
    MotorController& _motors;
    EncoderMonitor& _encoders;
    BatteryMonitor& _battery;

    WebSocketsServer* _ws;   // ❗ Pointer statt Objekt

    unsigned long _lastCommandTime;
    unsigned long _lastTelemetry;

    static CommsManager* _instance;

    static void eventTrampoline(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
    void onEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);

    void handleCommand(const char* payload);
    void sendTelemetry();
};
 */




/* #pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include "config.h"
#include "motor_controller.h"
#include "encoder_monitor.h"
#include "battery_monitor.h"

class CommsManager {
public:
    CommsManager(MotorController& motors, EncoderMonitor& encoders, BatteryMonitor& battery);
    void begin();
    void loop();
    bool isCommandTimedOut() const;

private:
    MotorController& _motors;
    EncoderMonitor& _encoders;
    BatteryMonitor& _battery;
    WebSocketsServer _ws;
    unsigned long _lastCommandTime;
    unsigned long _lastTelemetry;

    void onEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
    void handleCommand(const char* payload);
    void sendTelemetry();

    static CommsManager* _instance;
    static void eventTrampoline(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
};
 */
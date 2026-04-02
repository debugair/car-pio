// comms_manager.h — Schnittstelle für WLAN, WebSocket und Telemetrie
#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include "motor_controller.h"
#include "encoder_monitor.h"
#include "config.h"

class CommsManager {
public:
    // Konstruktor: braucht Zugriff auf Motoren und Encoder für Steuerung + Telemetrie
    CommsManager(MotorController& motors, EncoderMonitor& encoders);

    void begin();  // WLAN verbinden + WebSocket-Server starten
    void loop();   // Muss in loop() aufgerufen werden — verarbeitet eingehende Nachrichten

    // Gibt true zurück wenn seit DEADMAN_TIMEOUT_MS kein Fahrbefehl ankam
    bool isCommandTimedOut() const;

private:
    MotorController&   _motors;        // Referenz auf den Motor-Controller
    EncoderMonitor&    _encoders;      // Referenz auf den Encoder-Monitor (für Telemetrie)
    WebSocketsServer*  _ws;            // WebSocket-Server (Pointer, wird in begin() erzeugt)
    unsigned long      _lastCommandTime; // Zeitstempel des letzten Fahrbefehls (ms)
    unsigned long      _lastTelemetry;   // Zeitstempel der letzten Telemetrie-Sendung (ms)

    // Singleton-Instanz — notwendig weil WebSocketsServer nur statische Callbacks erlaubt
    static CommsManager* _instance;

    // Statischer Trampolin-Callback leitet WebSocket-Events an onEvent() weiter
    static void eventTrampoline(uint8_t num, WStype_t type, uint8_t* payload, size_t length);

    void onEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
    void sendTelemetry(); // Sendet RPM-Daten an alle verbundenen Browser
};

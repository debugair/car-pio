// comms_manager.cpp — WLAN-Verbindung, WebSocket-Server und Telemetrie
// Empfängt Fahrbefehle vom Browser und schickt Raddrehzahl zurück.

#include "comms_manager.h"

// Zeiger auf die einzige Instanz — wird für den statischen ISR-Callback benötigt
CommsManager* CommsManager::_instance = nullptr;

CommsManager::CommsManager(MotorController& motors, EncoderMonitor& encoders)
    : _motors(motors), _encoders(encoders),
      _ws(nullptr), _lastCommandTime(0), _lastTelemetry(0)
{
    _instance = this;
}

void CommsManager::begin() {
    // Statische IP aus config.h setzen, falls dort definiert.
    // Zum Deaktivieren: alle vier STATIC_*-Zeilen in config.h auskommentieren.
#ifdef STATIC_IP
    IPAddress ip(STATIC_IP), gateway(STATIC_GATEWAY);
    IPAddress subnet(STATIC_SUBNET), dns(STATIC_DNS);
    WiFi.config(ip, gateway, subnet, dns);
    Serial.printf("Statische IP: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
#endif

    // Mit WLAN verbinden (SSID und Passwort aus config.h)
    Serial.printf("Verbinde mit WLAN '%s'", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("\nVerbunden! IP: %s\n", WiFi.localIP().toString().c_str());

    // WebSocket-Server starten — Browser verbindet sich hier für Echtzeit-Steuerung
    _ws = new WebSocketsServer(WEBSOCKET_PORT);
    _ws->begin();
    _ws->onEvent(eventTrampoline); // eventTrampoline leitet Events an onEvent() weiter
}

void CommsManager::loop() {
    // WebSocket-Verbindungen bedienen (muss regelmäßig aufgerufen werden)
    if (_ws) _ws->loop();

    // Telemetrie-Daten (RPM) periodisch an den Browser senden
    sendTelemetry();
}

bool CommsManager::isCommandTimedOut() const {
    // Gibt true zurück wenn seit DEADMAN_TIMEOUT_MS kein Fahrbefehl ankam
    return millis() - _lastCommandTime > DEADMAN_TIMEOUT_MS;
}

void CommsManager::sendTelemetry() {
    // Nur alle TELEMETRY_INTERVAL_MS senden, nicht bei jedem loop()-Aufruf
    if (millis() - _lastTelemetry < TELEMETRY_INTERVAL_MS) return;
    _lastTelemetry = millis();

    // Format: "T:linksRPM,rechtsRPM" — Browser wertet das aus und zeigt es an
    char buf[64];
    snprintf(buf, sizeof(buf), "T:%.1f,%.1f",
             _encoders.getLeftRPM(), _encoders.getRightRPM());
    if (_ws) _ws->broadcastTXT(buf);
}

void CommsManager::onEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {

        case WStype_CONNECTED: {
            // Ein Browser hat sich verbunden
            IPAddress ip = _ws->remoteIP(num);
            Serial.printf("Client %u verbunden von %s\n", num, ip.toString().c_str());
            break;
        }

        case WStype_DISCONNECTED:
            // Browser hat die Verbindung getrennt
            Serial.printf("Client %u getrennt\n", num);
            break;

        case WStype_TEXT: {
            // Fahrbefehl empfangen: Format "links,rechts" (z.B. "200,-150")
            // Wertebereich: -255 (voll rückwärts) bis 255 (voll vorwärts)
            int left = 0, right = 0;
            if (sscanf((char*)payload, "%d,%d", &left, &right) == 2) {
                _motors.setLeft(left);
                _motors.setRight(right);
                _lastCommandTime = millis(); // Totmann-Timer zurücksetzen
            }
            break;
        }

        default:
            break;
    }
}

// Statischer Callback — WebSocketsServer kann keine Methoden direkt aufrufen,
// deshalb leiten wir hier auf die echte onEvent()-Methode weiter.
void CommsManager::eventTrampoline(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    if (_instance) _instance->onEvent(num, type, payload, length);
}

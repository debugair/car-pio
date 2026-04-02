// main.cpp — Einstiegspunkt des ESP32-Programms
// Hier wird alles gestartet: Motoren, Encoder, WLAN, WebSocket und Webserver.

#include <Arduino.h>
#include <WebServer.h>
#include <LittleFS.h>
#include "motor_controller.h"
#include "encoder_monitor.h"
#include "comms_manager.h"

// Globale Objekte — werden einmal erzeugt und dann überall genutzt
MotorController motors;          // Steuert die zwei Motoren
EncoderMonitor  encoders;        // Liest die Raddrehzahl per Interrupt
CommsManager    comms(motors, encoders); // Kümmert sich um WLAN und WebSocket
WebServer       httpServer(80);  // Liefert die Webseite an den Browser (Port 80)

void setup() {
    // Kurze Wartezeit damit der ESP32 sauber hochfährt
    delay(2000);
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n\n=== BOOT ===");

    // Alle Komponenten initialisieren
    motors.begin();    // GPIO-Pins für L298N konfigurieren
    encoders.begin();  // Encoder-Interrupts registrieren
    comms.begin();     // WLAN verbinden + WebSocket-Server starten

    // Dateisystem starten — hier liegen index.html, favicon.ico, icon.png
    if (!LittleFS.begin()) {
        Serial.println("LittleFS FEHLER: Dateisystem konnte nicht gemountet werden!");
    } else {
        Serial.println("LittleFS OK");
    }

    // Webserver-Routen: Jede URL wird direkt aus LittleFS bedient
    httpServer.serveStatic("/",            LittleFS, "/index.html", "text/html");
    httpServer.serveStatic("/favicon.ico", LittleFS, "/favicon.ico", "image/x-icon");
    httpServer.serveStatic("/icon.png",    LittleFS, "/icon.png",    "image/png");
    httpServer.begin();

    Serial.println("Webserver gestartet (Port 80)");
    Serial.printf("WebSocket gestartet (Port %d)\n", WEBSOCKET_PORT);
    Serial.println("=== BEREIT ===");
}

void loop() {
    // HTTP-Anfragen des Browsers verarbeiten (z.B. Seite laden)
    httpServer.handleClient();

    // Encoder-Pulses auswerten und RPM berechnen
    encoders.update();

    // WebSocket-Nachrichten empfangen + Telemetrie senden
    comms.loop();

    // Totmann-Schalter: Wenn seit 250ms kein Befehl kam → Motoren stoppen
    // Schützt davor, dass das Auto unkontrolliert weiterfährt bei Verbindungsabbruch
    if (comms.isCommandTimedOut()) motors.stop();

    delay(1);
}

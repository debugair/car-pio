/*
HTML auf ESP32 flashen

PlatformIO kann Webseiten in den Flash laden.
pio run -t uploadfs



lib_deps =
    links2004/WebSockets@^2.6.1
    me-no-dev/ESP Async WebServer
    me-no-dev/AsyncTCP




. Minimaler ESP32 Webserver

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <WebSocketsServer.h>
#include <LittleFS.h>

const char* ssid = "4050";
const char* password = "xxxx";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {

    AwsFrameInfo *info = (AwsFrameInfo*)arg;

    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {

        data[len] = 0;
        String msg = (char*)data;

        Serial.println(msg);

        int left,right;

        if(sscanf(msg.c_str(), "%d,%d", &left, &right)==2){

            Serial.printf("Motor L:%d R:%d\n", left, right);

            // hier deine Motorsteuerung
        }
    }
}

void onWsEvent(AsyncWebSocket * server,
               AsyncWebSocketClient * client,
               AwsEventType type,
               void * arg,
               uint8_t *data,
               size_t len) {

    switch(type) {

        case WS_EVT_CONNECT:
            Serial.printf("Client %u connected\n", client->id());
        break;

        case WS_EVT_DISCONNECT:
            Serial.printf("Client %u disconnected\n", client->id());
        break;

        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, len);
        break;
    }
}

void setup() {

    Serial.begin(115200);

    WiFi.begin(ssid,password);

    while(WiFi.status()!=WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println(WiFi.localIP());

    LittleFS.begin();

    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

    server.begin();
}

void loop() {
}










*/








// main.cpp
#include <Arduino.h>
#include "motor_controller.h"
#include "encoder_monitor.h"
#include "comms_manager.h"

MotorController motors;
EncoderMonitor encoders;
CommsManager comms(motors, encoders);









void setup() {

    delay(2000);
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n\n=== BOOT ===");
    Serial.println("Setup start...");

    motors.begin();
    encoders.begin();
    comms.begin();

    Serial.printf("WebSocket server started on port %d\n", WEBSOCKET_PORT);
    Serial.println("Setup done!");
}




/* void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Setup start...");

    motors.begin();
    encoders.begin();
    comms.begin();
    Serial.printf("WebSocket server started on port %d\n", WEBSOCKET_PORT);
    Serial.println("Setup done!");
}
 */
void loop() {
    encoders.update();
    comms.loop();
    delay(1);
}





/* #include <Arduino.h>
#include "motor_controller.h"
#include "encoder_monitor.h"
#include "battery_monitor.h"
#include "comms_manager.h"

// ❗ Nur Pointer, KEINE globalen Objekte
MotorController* motors;
EncoderMonitor* encoders;
BatteryMonitor* battery;
CommsManager* comms;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== BOOT ===");

    // Objekte erzeugen (jetzt ist ESP bereit)
    motors = new MotorController();
    encoders = new EncoderMonitor();
    battery = new BatteryMonitor();
    comms = new CommsManager(*motors, *encoders, *battery);

    Serial.println("Objects created");

    // Initialisierung
    motors->begin();
    encoders->begin();
    battery->begin();
    comms->begin();

    Serial.println("=== SETUP DONE ===");
}

void loop() {
    encoders->update();
    battery->update();
    comms->loop();

    if (comms->isCommandTimedOut()) {
        motors->stop();
    }

    delay(5); // wichtig für Stabilität
}



 */
/* #include <Arduino.h>
#include "motor_controller.h"
#include "encoder_monitor.h"
#include "battery_monitor.h"
#include "comms_manager.h"

MotorController motors;
EncoderMonitor encoders;
BatteryMonitor battery;
CommsManager comms(motors, encoders, battery);
void beforeSetup() __attribute__((constructor));
void beforeSetup() {
    Serial.begin(115200);
    Serial.println("BEFORE SETUP");
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.printf("setup start... \n ");
    // motors.begin();
    // encoders.begin();
    // battery.begin();
    // comms.begin();
    // Serial.printf("setup done \n");
    
}

void loop() {
    delay(1000);
    // encoders.update();
    // battery.update();
    // comms.loop();

    // if (comms.isCommandTimedOut()) {
    //     motors.stop();
    // }
}
 */
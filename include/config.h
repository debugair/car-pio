#pragma once

// --- Wi-Fi ---
#define WIFI_SSID       "DeinSSID"
#define WIFI_PASSWORD   "DeinPasswort"

// --- WebSocket ---
#define WEBSOCKET_PORT  81

// --- Left Motor (L298N) ---
#define MOTOR_LEFT_ENA  17   // PWM speed control
#define MOTOR_LEFT_IN1  14   // Direction A
#define MOTOR_LEFT_IN2  15   // Direction B

// --- Right Motor (L298N) ---
#define MOTOR_RIGHT_ENB 16   // PWM speed control
#define MOTOR_RIGHT_IN3  4   // Direction A
#define MOTOR_RIGHT_IN4  5   // Direction B

// --- Encoders (single-channel, 20 slots per revolution) ---
#define ENCODER_LEFT     6
#define ENCODER_RIGHT    7
#define ENCODER_SLOTS   20

// --- Telemetry ---
#define TELEMETRY_INTERVAL_MS 100  // Send RPM data every 100ms

// --- Deadman's Handle ---
#define DEADMAN_TIMEOUT_MS 250

// --- Static IP (comment out all four to use DHCP) ---
#define STATIC_IP      192,168,5,41
#define STATIC_GATEWAY 192,168,5,1
#define STATIC_SUBNET  255,255,255,0
#define STATIC_DNS     192,168,5,1

// --- Battery Monitoring ---
#define BATTERY_ADC_PIN        8     // GPIO for voltage divider output
#define BATTERY_DIVIDER_RATIO  2.0   // Voltage divider multiplier (e.g. 2:1 = 2.0)
#define BATTERY_FULL_VOLTAGE   6.0   // Fully charged voltage
#define BATTERY_LOW_VOLTAGE    4.5   // Low battery warning threshold
#define BATTERY_SAMPLES        16    // ADC readings to average (noise reduction)

// --- Motor PWM ---
#define PWM_FREQUENCY  1000  // Hz
#define PWM_RESOLUTION    8  // 8-bit = 0-255

#ifndef EXTERNS_H
#define EXTERNS_H

// #include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
// #include <WebSocketsServer.h>

#include <ESP32Servo.h>
#include <FastLED.h>

#include "accelerometer.h"

bool fullyOpened = false;
// bool myDFPlayerSetup = false;

AsyncWebServer server = AsyncWebServer(80);

Accelerometer accelerometer;
Servo wingServo;
Servo rotateServo;

// SoftwareSerial mySoftwareSerial(D5, D6);  // RX, TX
// DFRobotDFPlayerMini myDFPlayer;

CRGB leds[NUM_LEDS];

#endif
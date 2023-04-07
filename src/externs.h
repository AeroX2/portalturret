#ifndef EXTERNS_H
#define EXTERNS_H

#include <ESP32Servo.h>
#include <FastLED.h>

#include "accelerometer.h"

static bool diagnosing = false;
static int diagnoseAction = -1;

static int currentMoveSpeed = 0;
static int currentRotateAngle = 90;
static int8_t currentRotateDirection = 0;
static bool fullyOpened = false;
// bool myDFPlayerSetup = false;

static Accelerometer accelerometer;
static Servo wingServo;
static Servo rotateServo;

// static SoftwareSerial mySoftwareSerial(D5, D6);  // RX, TX
// static DFRobotDFPlayerMini myDFPlayer;

static CRGB leds[NUM_LEDS];

#endif
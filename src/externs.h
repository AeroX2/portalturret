#ifndef EXTERNS_H
#define EXTERNS_H

#include <HardwareSerial.h>
#include <ESP32Servo.h>
#include <FastLED.h>

#include "consts.h"
#ifdef USE_AUDIO
#include <DFRobotDFPlayerMini.h>
#endif

#include "accelerometer.h"

static bool diagnosing = false;
static int diagnoseAction = -1;

static int currentMoveSpeed = 0;
static int currentRotateAngle = 90;
static int8_t currentRotateDirection = 0;
static bool fullyOpened = false;

static Accelerometer accelerometer;
static Servo wingServo;
static Servo rotateServo;

static HardwareSerial hardwareSerial(1);
static DFRobotDFPlayerMini musicPlayer;

static CRGB leds[NUM_LEDS];

#endif
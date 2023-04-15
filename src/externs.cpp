#include "externs.h"

Accelerometer accelerometer;
Servo wingServo;
Servo rotateServo;

HardwareSerial hardwareSerial(1);
DFRobotDFPlayerMini musicPlayer;

CRGB leds[NUM_LEDS];
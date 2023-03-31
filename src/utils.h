#include "consts.h"

bool isMotionDetected() { return digitalRead(MOTION_SENSOR_PIN) == HIGH; }
bool isOpen() { return digitalRead(WING_SWITCH_PIN) == HIGH; }
bool isPlayingAudio() { return analogRead(A0) < 512; }
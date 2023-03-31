#ifndef UTILS_H
#define UTILS_H

#include "consts.h"

//For some reason we need to cache this value, as checking it every loop causes the webserver to freeze.
//https://github.com/me-no-dev/ESPAsyncWebServer/issues/944
bool isDetectingMotionCached = false;
unsigned long lastMotionCheck = 0;
inline bool isDetectingMotion() {
  unsigned long curMillis = millis();
  if (curMillis > lastMotionCheck + 50) {
    isDetectingMotionCached = analogRead(MOTION_SENSOR_PIN) > 512;
    lastMotionCheck = curMillis;
  }
  return isDetectingMotionCached;
}

bool isOpen() { return digitalRead(WING_SWITCH_PIN) == HIGH; }
bool isPlayingAudio() { return analogRead(A0) < 512; }

#endif
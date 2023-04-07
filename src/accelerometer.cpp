#include "accelerometer.h"

void Accelerometer::setup() {
  pinMode(ACCELEROMETER_PIN_X, INPUT);
  pinMode(ACCELEROMETER_PIN_Y, INPUT);
  pinMode(ACCELEROMETER_PIN_Z, INPUT);
}

void Accelerometer::update() {
  // sensors_event_t event;
  // accel.getEvent(&event);

  smoothX -= accelX[currentMeasurement];
  accelX[currentMeasurement] = getRawX();
  smoothX += accelX[currentMeasurement];

  smoothY -= accelY[currentMeasurement];
  accelY[currentMeasurement] = getRawY();
  smoothY += accelY[currentMeasurement];

  smoothZ -= accelZ[currentMeasurement];
  accelZ[currentMeasurement] = getRawZ();
  smoothZ += accelZ[currentMeasurement];

  currentMeasurement++;
  if (currentMeasurement >= ACCEL_MEASUREMENTS) {
    buffered = true;
    currentMeasurement = 0;
  }
}

uint16_t Accelerometer::getRawX() { return analogRead(ACCELEROMETER_PIN_X); }

uint16_t Accelerometer::getRawY() { return analogRead(ACCELEROMETER_PIN_Y); }

uint16_t Accelerometer::getRawZ() { return analogRead(ACCELEROMETER_PIN_Z); }

float_t Accelerometer::getRealX() { return ((smoothZ / ACCEL_MEASUREMENTS) - 2048 / 330.0); }

float_t Accelerometer::getRealY() { return ((smoothZ / ACCEL_MEASUREMENTS) - 2048 / 330.0); }

float_t Accelerometer::getRealZ() { return ((smoothZ / ACCEL_MEASUREMENTS) - 2048 / 330.0); }
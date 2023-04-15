#include "accelerometer.h"

void Accelerometer::setup() {
  pinMode(ACCELEROMETER_PIN_X, INPUT);
  pinMode(ACCELEROMETER_PIN_Y, INPUT);
  pinMode(ACCELEROMETER_PIN_Z, INPUT);

  for (int i = 0; i < ACCEL_MEASUREMENTS; i++) {
    accelX[i] = 0;
    accelY[i] = 0;
    accelZ[i] = 0;
  }
}

void Accelerometer::update() {
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

uint16_t Accelerometer::getRawX() {
  int32_t x = analogRead(ACCELEROMETER_PIN_X);
  // if (x < minX) minX = x;
  // if (x > maxX) maxX = x;
  return x;
}

uint16_t Accelerometer::getRawY() {
  int32_t y = analogRead(ACCELEROMETER_PIN_Y);
  // if (y < minY) minY = y;
  // if (y > maxY) maxY = y;
  return y;
}

uint16_t Accelerometer::getRawZ() {
  int32_t z = analogRead(ACCELEROMETER_PIN_Z);
  // if (z < minZ) minZ = z;
  // if (z > maxZ) maxZ = z;
  return z;
}

float_t Accelerometer::getRealX() {
  return map((smoothX / ACCEL_MEASUREMENTS), minX, maxX, -1000, 1000) / 1000.0;
}

float_t Accelerometer::getRealY() {
  return map((smoothY / ACCEL_MEASUREMENTS), minY, maxY, -1000, 1000) / 1000.0;
}

float_t Accelerometer::getRealZ() {
  return map((smoothZ / ACCEL_MEASUREMENTS), minZ, maxZ, -1000, 1000) / 1000.0;
}

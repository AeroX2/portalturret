const int measurements = 5;
int currentMeasurement = 0;
bool accelerometerBuffered = false;
int16_t accelX[measurements];
int16_t accelY[measurements];
int16_t accelZ[measurements];
int32_t smoothX = 0;
int32_t smoothY = 0;
int32_t smoothZ = 0;

uint16_t getRawX() {
  return analogRead(ACCELEROMETER_PIN_X);
}

uint16_t getRawY() {
  return analogRead(ACCELEROMETER_PIN_Y);
}

uint16_t getRawZ() {
  return analogRead(ACCELEROMETER_PIN_Z);
}

void setupAccelerometer() {
  pinMode(ACCELEROMETER_PIN_X, INPUT);
  pinMode(ACCELEROMETER_PIN_Y, INPUT);
  pinMode(ACCELEROMETER_PIN_Z, INPUT);

  analogReadResolution(10);
}

void updateAccelerometer() {
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
    if (currentMeasurement >= measurements) {
      accelerometerBuffered = true;
      currentMeasurement = 0;
    }
}
#include <Arduino.h>
#include "consts.h"

class Accelerometer {
 public:
  void setup();
  void update();

  float_t getRealX();
  float_t getRealY();
  float_t getRealZ();

  uint16_t getRawX();
  uint16_t getRawY();
  uint16_t getRawZ();

  bool buffered = false;
 private:

  int currentMeasurement = 0;
  int16_t accelX[ACCEL_MEASUREMENTS];
  int16_t accelY[ACCEL_MEASUREMENTS];
  int16_t accelZ[ACCEL_MEASUREMENTS];
  int32_t smoothX = 0;
  int32_t smoothY = 0;
  int32_t smoothZ = 0;

// minX: 1601 maxX: 2298
// minY: 1429 maxY: 2330
// minZ: 1315 maxZ: 2672

  int32_t minX = 1601;
  int32_t maxX = 2298;
  int32_t minY = 1429;
  int32_t maxY = 2330;
  int32_t minZ = 1315;
  int32_t maxZ = 2672;
};
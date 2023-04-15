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

  int32_t minX = 1437;
  int32_t maxX = 2571;
  int32_t minY = 1441;
  int32_t maxY = 2294;
  int32_t minZ = 1393;
  int32_t maxZ = 2496;
};
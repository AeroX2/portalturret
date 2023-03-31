#include <Arduino.h>

#include "consts.h"
#include "routines.h"
#include "utils.h"

enum class TurretMode { Automatic = 0, Manual = 1 };

enum class TurretState {
  Idle = 0,
  Activated = 1,
  Searching = 2,
  Engaging = 3,
  TargetLost = 4,
  PickedUp = 5,
  Shutdown = 6,
  ManualEngaging = 7,
  Rebooting = 8,
};

enum class ManualState {
  Idle,
  Opening,
  Closing,
  Firing,
};

class TurretStateBehaviour {
 public:
  void setState(TurretState nextState);
  void setManualState(ManualState nextState);

  TurretMode currentTurretMode;
  bool wasOpen;
  bool wingsOpen;

 private:
  void manualRotation(unsigned long deltaTime);
  void stateBehaviour();

  // bool alarm;
  bool shouldUpdate = false;

  TurretState currentState = TurretState::Idle;
  ManualState currentManualState = ManualState::Idle;

  unsigned long detectTime = 0;
  unsigned long undetectTime = 0;
  unsigned long previousTime = 0;
  unsigned long stateStartTime = 0;
  unsigned long lastMovementTime = 0;
};

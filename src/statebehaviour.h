#ifndef STATE_BEHAVIOUR_H
#define STATE_BEHAVIOUR_H

#include <Arduino.h>

#include "consts.h"
#include "externs.h"
#include "utils.h"

#include <AceRoutine.h>
using namespace ace_routine;

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
  void init();
  void update();

  void setState(TurretState nextState);
  void setManualState(ManualState nextState);
  void manualRotation(unsigned long deltaTime);

  TurretMode currentTurretMode;
  TurretState currentState = TurretState::Idle;

  bool wasOpen;
  bool wingsOpen;
  bool needsSetup;
 private:
  ManualState currentManualState = ManualState::Idle;

  unsigned long detectTime = 0;
  unsigned long undetectTime = 0;
  unsigned long previousTime = 0;
  unsigned long stateStartTime = 0;
  unsigned long lastMovementTime = 0;
};

extern TurretStateBehaviour state;

#endif
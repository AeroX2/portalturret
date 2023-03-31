#include "statebehaviour.h"

void TurretStateBehaviour::init() { previousTime = millis(); }

void TurretStateBehaviour::update() {
  unsigned long deltaTime = millis() - previousTime;
  previousTime = millis();

  if (currentTurretMode == TurretMode::Manual) {
    switch (currentManualState) {
      case ManualState::Idle:
        manualRotation(deltaTime);
        break;
      case ManualState::Opening:
        openWingsRoutine.runCoroutine();
        if (openWingsRoutine.isDone()) {
          setManualState(ManualState::Idle);
        }
        break;
      case ManualState::Closing:
        closeWingsRoutine.runCoroutine();
        if (closeWingsRoutine.isDone()) {
          setManualState(ManualState::Idle);
        }
        break;
      case ManualState::Firing:
        manualRotation(deltaTime);
        manualEngagingRoutine.runCoroutine();
        if (manualEngagingRoutine.isDone()) {
          setManualState(ManualState::Idle);
        }
        break;
    }
  }
  if (currentTurretMode == TurretMode::Automatic) {
    bool motionDetected = isDetectingMotion();
    float zMovement = accelerometer.getRealZ();
    bool pickedUp = accelerometer.buffered && (zMovement < 8 || zMovement > 12);
    bool movedAround =
        accelerometer.buffered && (zMovement < 9.5 || zMovement > 10.5);
    bool onItsSide = accelerometer.buffered && (zMovement < 5);
    if (movedAround) {
      lastMovementTime = millis();
    }

    if (pickedUp && currentState != TurretState::PickedUp &&
        currentState != TurretState::Shutdown &&
        currentState != TurretState::Rebooting) {
      setState(TurretState::PickedUp);
    }

    switch (currentState) {
      case TurretState::Idle:
        if (motionDetected) {
          setState(TurretState::Activated);
        }
        break;
      case TurretState::Activated:
        activatedRoutine.runCoroutine();
        if (activatedRoutine.isDone()) {
          if (motionDetected) {
            setState(TurretState::Engaging);
          } else {
            setState(TurretState::Searching);
          }
        }
        break;
      case TurretState::Searching:
        searchingRoutine.runCoroutine();
        if (millis() > stateStartTime + 10000) {
          setState(TurretState::TargetLost);
        }
        if (motionDetected && millis() > stateStartTime + 100) {
          setState(TurretState::Engaging);
        }
        break;
      case TurretState::TargetLost:
        targetLostRoutine.runCoroutine();
        if (targetLostRoutine.isDone()) {
          setState(TurretState::Idle);
        }
        break;
      case TurretState::Engaging:
        engagingRoutine.runCoroutine();
        if (engagingRoutine.isDone()) {
          if (wingsOpen) {
            setState(TurretState::Searching);
          } else {
            setState(TurretState::Idle);
          }
        }
        break;
      case TurretState::PickedUp:
        pickedUpRoutine.runCoroutine();
        if (onItsSide) {
          setState(TurretState::Shutdown);
        } else if (!movedAround && millis() > lastMovementTime + 5000) {
          setState(TurretState::Activated);
        }
        break;
      case TurretState::Shutdown:
        shutdownRoutine.runCoroutine();
        if (shutdownRoutine.isDone() && !onItsSide) {
          setState(TurretState::Rebooting);
        }
        break;
      case TurretState::Rebooting:
        rebootRoutine.runCoroutine();
        if (rebootRoutine.isDone()) {
          setState(TurretState::Idle);
        }
        break;
    }
  }
}

void TurretStateBehaviour::setState(TurretState nextState) {
  // Stop the Wing Servos just in case;
  wingServo.write(STATIONARY_ANGLE);

  if (currentTurretMode == TurretMode::Automatic) {
    switch (nextState) {
      case TurretState::Activated:
        activatedRoutine.reset();
        break;
      case TurretState::Engaging:
        engagingRoutine.reset();
        break;
      case TurretState::Searching:
        searchingRoutine.reset();
        break;
      case TurretState::TargetLost:
        targetLostRoutine.reset();
        break;
      case TurretState::PickedUp:
        pickedUpRoutine.reset();
        break;
      case TurretState::Shutdown:
        shutdownRoutine.reset();
        break;
      case TurretState::ManualEngaging:
        manualEngagingRoutine.reset();
        break;
      case TurretState::Rebooting:
        rebootRoutine.reset();
        break;
    }
    stateStartTime = millis();
    currentState = nextState;
  }
}

void TurretStateBehaviour::setManualState(ManualState nextState) {
  // Stop the Wing Servos just in case;
  wingServo.write(STATIONARY_ANGLE);

  if (currentTurretMode == TurretMode::Manual) {
    switch (nextState) {
      case ManualState::Opening:
        openWingsRoutine.reset();
        break;
      case ManualState::Closing:
        closeWingsRoutine.reset();
        break;
      case ManualState::Firing:
        manualEngagingRoutine.reset();
        break;
    }
    currentManualState = nextState;
  }
}

void TurretStateBehaviour::manualRotation(unsigned long deltaTime) {
  manualMovementRoutine.runCoroutine();
}
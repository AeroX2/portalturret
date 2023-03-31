#ifndef ROUTINES_H
#define ROUTINES_H

#include <FastLED.h>
#include <AceRoutine.h>
#include "consts.h"
#include "utils.h"
#include "externs.h"

// int8_t currentRotateDirection = 0;
// int currentRotateAngle = 90;

EXTERN_COROUTINE(openWingsRoutine);
EXTERN_COROUTINE(closeWingsRoutine) ;
EXTERN_COROUTINE(activatedRoutine) ;
EXTERN_COROUTINE(searchingRoutine) ;
EXTERN_COROUTINE(engagingRoutine) ;
EXTERN_COROUTINE(targetLostRoutine); 
EXTERN_COROUTINE(pickedUpRoutine) ;
EXTERN_COROUTINE(shutdownRoutine); 
EXTERN_COROUTINE(rebootRoutine) ;
EXTERN_COROUTINE(manualEngagingRoutine) ;
EXTERN_COROUTINE(manualMovementRoutine);

#endif
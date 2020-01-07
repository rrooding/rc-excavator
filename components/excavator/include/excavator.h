#ifndef EXCAVATOR
#define EXCAVATOR

#include "controller.h"
#include <stdbool.h>

typedef struct Excavator Excavator;

Excavator *ExcavatorCreate();
void ExcavatorFree(Excavator *this);

void ExcavatorControllerCallback(void *context, controller_event_t event);

void ExcavatorStart(Excavator *this);
void ExcavatorStop(Excavator *this);

void ExcavatorLeftTrackReverse(Excavator *this);
void ExcavatorLeftTrackForward(Excavator *this);
void ExcavatorRightTrackReverse(Excavator *this);
void ExcavatorRightTrackForward(Excavator *this);

void ExcavatorSetLeftTrackSpeed(Excavator *this, uint8_t speed);
void ExcavatorSetRightTrackSpeed(Excavator *this, uint8_t speed);

bool ExcavatorIsStarted(Excavator *this);

#endif

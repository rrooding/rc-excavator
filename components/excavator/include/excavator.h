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
bool ExcavatorIsStarted(Excavator *this);

#endif

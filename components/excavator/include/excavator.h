#ifndef EXCAVATOR
#define EXCAVATOR

#include <stdbool.h>

typedef struct Excavator Excavator;

Excavator *ExcavatorCreate();
void ExcavatorFree(Excavator *this);

void ExcavatorStart(Excavator *this);
void ExcavatorStop(Excavator *this);
bool ExcavatorIsStarted(Excavator *this);

#endif

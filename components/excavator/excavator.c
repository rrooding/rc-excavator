#include <stdlib.h>
#include <stdbool.h>
#include "excavator_internal.h"

Excavator *ExcavatorCreate() {
  Excavator *newExcavator = malloc(sizeof(Excavator));

  ExcavatorInitialize(newExcavator);

  return newExcavator;
}

void ExcavatorFree(Excavator *this) {
  free(this);
}

void ExcavatorInitialize(Excavator *this) {
  this->started = false;
}

void ExcavatorStart(Excavator *this) {
  this->started = true;
}

void ExcavatorStop(Excavator *this) {
  this->started = false;
}

bool ExcavatorIsStarted(Excavator *this) {
  return this->started;
}

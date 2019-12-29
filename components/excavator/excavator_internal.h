#ifndef EXCAVATOR_INTERNAL
#define EXCAVATOR_INTERNAL

#include "excavator.h"

struct Excavator {
  bool started;
};

void ExcavatorInitialize(Excavator *this);

#endif

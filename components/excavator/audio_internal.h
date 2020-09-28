#ifndef EXCAVATOR_AUDIO_INTERNAL
#define EXCAVATOR_AUDIO_INTERNAL

#include "audio.h"

struct Audio {
  int i2s_num;
  uint32_t index;
};

void AudioInitialize(Audio *this);

#endif

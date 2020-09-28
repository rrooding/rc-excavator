#ifndef EXCAVATOR_AUDIO
#define EXCAVATOR_AUDIO

typedef struct Audio Audio;

Audio *AudioCreate();
void AudioFree(Audio *this);

void AudioPlay(Audio *this);

#endif

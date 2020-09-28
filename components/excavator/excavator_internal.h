#ifndef EXCAVATOR_INTERNAL
#define EXCAVATOR_INTERNAL

#include "excavator.h"
#include "servo.h"
#include "audio.h"

typedef struct {
  Servo *leftTrack;
  Servo *rightTrack;
} excavator_motors_t;

struct Excavator {
  bool started;
  bool left_track_forward;
  bool right_track_forward;
  int8_t left_track_speed;
  int8_t right_track_speed;
  excavator_motors_t motors;
  Audio *audio;
};

void ExcavatorInitialize(Excavator *this);

// Calculate the track speed based on the input speed and the direction. Input is a
// speed between 0 and 255, output is a speed between -127 and 0 when the direction is
// reverse, and 1 to 128 when the direction is forward.
int8_t ExcavatorCalculateTrackSpeed(bool forward, uint8_t speed);

#endif

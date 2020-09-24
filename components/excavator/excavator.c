#include <stdlib.h>

#include "esp_log.h"
#include "excavator_internal.h"
#include "pin_bindings.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "EX_EXCAVATOR"

Excavator *ExcavatorCreate() {
  Excavator *newExcavator = malloc(sizeof(Excavator));

  ExcavatorInitialize(newExcavator);

  return newExcavator;
}

void ExcavatorFree(Excavator *this) { 
  ServoFree(this->motors.leftTrack);
  ServoFree(this->motors.rightTrack);
  free(this);
}

void ExcavatorInitialize(Excavator *this) {
  this->started = false;

  this->left_track_forward = true;
  this->right_track_forward = true;

  this->left_track_speed = 0;
  this->right_track_speed = 0;

  this->motors.leftTrack = ServoCreate(LEFT_TRACK);
  this->motors.rightTrack = ServoCreate(RIGHT_TRACK);
}

void ExcavatorControllerCallback(void *context, controller_event_t event) {
  Excavator *this = (Excavator *)context;

  // Start / stop
  if(event.start.button_up) ExcavatorIsStarted(this) ? ExcavatorStop(this) : ExcavatorStart(this);

  // Don't handle other inputs unless the excavator is started
  if(!ExcavatorIsStarted(this)) return;

  // TRACKS
  //
  // Set direction of tracks
  if(event.l1.button_down) ExcavatorLeftTrackReverse(this);
  if(event.l1.button_up) ExcavatorLeftTrackForward(this);
  if(event.r1.button_down) ExcavatorRightTrackReverse(this);
  if(event.r1.button_up) ExcavatorRightTrackForward(this);

  // Set speed for left and right tracks.
  if(event.l2.analog) ExcavatorSetLeftTrackSpeed(this, event.l2.analog);
  if(event.r2.analog) ExcavatorSetRightTrackSpeed(this, event.r2.analog);

  // When trigger is released, set speed back to 0
  if(event.l2.button_up) {
    vTaskDelay(100 / portTICK_PERIOD_MS);
    ExcavatorSetLeftTrackSpeed(this, 0);
  }
  if(event.r2.button_up) {
    vTaskDelay(100 / portTICK_PERIOD_MS);
    ExcavatorSetRightTrackSpeed(this, 0);
  }
}

bool ExcavatorIsStarted(Excavator *this) { return this->started; }

// Temporary
#include <driver/dac.h>

void ExcavatorStart(Excavator *this) {
  this->started = true;

  // Temporary for testing the DAC
  dac_output_enable(SPEAKER);
  dac_output_voltage(SPEAKER, 200); // output should be ~2.59V
}

void ExcavatorStop(Excavator *this) {
  this->started = false;

  dac_output_voltage(SPEAKER, 0);
  dac_output_disable(SPEAKER);
}

void ExcavatorLeftTrackReverse(Excavator *this) { this->left_track_forward = false; }
void ExcavatorLeftTrackForward(Excavator *this) { this->left_track_forward = true; }
void ExcavatorRightTrackReverse(Excavator *this) { this->right_track_forward = false; }
void ExcavatorRightTrackForward(Excavator *this) { this->right_track_forward = true; }

void ExcavatorSetLeftTrackSpeed(Excavator *this, uint8_t speed) {
  this->left_track_speed = ExcavatorCalculateTrackSpeed(this->left_track_forward, speed);
  ServoSetSpeed(this->motors.leftTrack, this->left_track_speed);
}

void ExcavatorSetRightTrackSpeed(Excavator *this, uint8_t speed) {
  // Note the inversion of the calculated speed, because the right servo is installed
  // in reverse, compared to the left servo. We need to invert the speed
  this->right_track_speed = ExcavatorCalculateTrackSpeed(this->right_track_forward, speed);
  ServoSetSpeed(this->motors.rightTrack, this->right_track_speed);
}

int8_t ExcavatorCalculateTrackSpeed(bool forward, uint8_t speed) {
  if(speed == 0)
    return speed;

  uint8_t in_min = 1;
  uint8_t in_max = 255;
  uint8_t in_range = in_max - in_min;

  uint8_t out_min = 1;
  uint8_t out_max = 127;
  uint8_t out_range = out_max - out_min;

  uint8_t new_speed = (((speed - in_min) * out_range) / in_range) + out_min;

  if(!forward) {
    new_speed = -new_speed;
  }

  return new_speed;
}

#include "esp_log.h"
#include "excavator_internal.h"
#include <stdlib.h>

#define TAG "EX_EXCAVATOR"

Excavator *ExcavatorCreate() {
  Excavator *newExcavator = malloc(sizeof(Excavator));

  ExcavatorInitialize(newExcavator);

  return newExcavator;
}

void ExcavatorFree(Excavator *this) { 
  ServoFree(this->motors.swing);
  free(this);
}

void ExcavatorInitialize(Excavator *this) {
  this->started = false;

  this->left_track_forward = true;
  this->right_track_forward = true;

  this->left_track_speed = 0;
  this->right_track_speed = 0;

  this->motors.swing = ServoCreate();
}

void ExcavatorControllerCallback(void *context, controller_event_t event) {
  Excavator *this = (Excavator *)context;

  if (event.l1.button_down) { ExcavatorLeftTrackReverse(this); }
  if (event.r1.button_down) { ExcavatorRightTrackReverse(this); }

  if (event.l1.button_up) { ExcavatorLeftTrackForward(this); }
  if (event.r1.button_up) { ExcavatorRightTrackForward(this); }

  // Set speed for left and right tracks. When trigger is released, set speed
  // back to 0 if (event.l2.analog) { ExcavatorSetLeftTrackSpeed(this, event.l2.analog); }
  if (event.l2.button_up) { ExcavatorSetLeftTrackSpeed(this, 0); }
  if (event.r2.analog) { ExcavatorSetRightTrackSpeed(this, event.r2.analog); }
  if (event.r2.button_up) { ExcavatorSetRightTrackSpeed(this, 0); }

  // if (ps3.analog.stick.lx) {
  //   //ESP_LOGI(TAG, "Event detected %i", ps3.analog.stick.lx);
  //   set_pulse(0, stick_to_pulse(ps3.analog.stick.lx));
  // }

  // if (ps3.analog.button.l2) {
  //   ESP_LOGI(TAG, "Event detected %i", ps3.analog.button.l2);
  //   set_pulse(1, button_to_pulse(ps3.analog.button.l2));
  // }

  // if (event.button_up.l2) {
  //   ESP_LOGI(TAG, "Stop left track");
  //   set_pulse(1, button_to_pulse(0));
  // }

  // if (ps3.analog.button.r2) {
  //   ESP_LOGI(TAG, "Event detected %i", ps3.analog.button.r2);
  //   set_pulse(2, button_to_pulse(ps3.analog.button.r2));
  // }

  // if (event.button_up.r2) {
  //   ESP_LOGI(TAG, "Stop right track");
  //   set_pulse(2, button_to_pulse(0));
  // }
}

void ExcavatorStart(Excavator *this) { this->started = true; }

void ExcavatorStop(Excavator *this) { this->started = false; }

void ExcavatorLeftTrackReverse(Excavator *this) { this->left_track_forward = false; }
void ExcavatorLeftTrackForward(Excavator *this) { this->left_track_forward = true; }
void ExcavatorRightTrackReverse(Excavator *this) { this->right_track_forward = false; }
void ExcavatorRightTrackForward(Excavator *this) { this->right_track_forward = true; }

void ExcavatorSetLeftTrackSpeed(Excavator *this, uint8_t speed) {
  this->left_track_speed = ExcavatorCalculateTrackSpeed(this->left_track_forward, speed);
}

void ExcavatorSetRightTrackSpeed(Excavator *this, uint8_t speed) {
  this->right_track_speed = ExcavatorCalculateTrackSpeed(this->right_track_forward, speed);
}

int8_t ExcavatorCalculateTrackSpeed(bool forward, uint8_t speed) {
  if (forward) {
  } else {
  }
}

bool ExcavatorIsStarted(Excavator *this) { return this->started; }

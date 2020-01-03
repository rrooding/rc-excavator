#include "esp_log.h"
#include "excavator_internal.h"
#include <stdlib.h>

#define TAG "EX_EXCAVATOR"

Excavator *ExcavatorCreate() {
  Excavator *newExcavator = malloc(sizeof(Excavator));

  ExcavatorInitialize(newExcavator);

  return newExcavator;
}

void ExcavatorFree(Excavator *this) { free(this); }

void ExcavatorInitialize(Excavator *this) { this->started = false; }

void ExcavatorControllerCallback(void *context, controller_event_t event) {
  Excavator *this = (Excavator *)context;

  if (event.l2.analog) {
    ESP_LOGI(TAG, "l2 %i", event.l2.analog);
  }

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

bool ExcavatorIsStarted(Excavator *this) { return this->started; }

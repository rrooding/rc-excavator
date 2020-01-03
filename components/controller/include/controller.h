#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "driver/gpio.h"

typedef struct {
  int8_t x;
  int8_t y;
} controller_analog_joystick_t;

typedef struct {
  bool button_up;
  bool button_down;
  uint8_t analog;
} controller_button_t;

typedef struct {
  controller_analog_joystick_t left_joystick;
  controller_analog_joystick_t right_joystick;

  controller_button_t up;
  controller_button_t down;
  controller_button_t left;
  controller_button_t right;

  controller_button_t l1;
  controller_button_t l2;
  controller_button_t r1;
  controller_button_t r2;

  controller_button_t start;
  controller_button_t select;

  controller_button_t a;
  controller_button_t b;
  controller_button_t x;
  controller_button_t y;
} controller_event_t;

typedef void (*controller_event_callback_t)(void *context,
                                            controller_event_t event);

void controller_init(gpio_num_t gpio_num);

void controller_set_event_callback(controller_event_callback_t cb,
                                   void *context);

bool controller_is_connected();

#endif

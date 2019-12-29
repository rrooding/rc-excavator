#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "driver/gpio.h"

typedef void(*controller_event_callback_t)();

void controller_init(gpio_num_t gpio_num);

void controller_set_event_callback(controller_event_callback_t cb);

bool controller_is_connected();

#endif

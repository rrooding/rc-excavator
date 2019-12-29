#ifndef CONTROLLER_GPIO_H
#define CONTROLLER_GPIO_H

#include "driver/gpio.h"

void controller_init_gpio(gpio_num_t gpio_num);
void controller_set_gpio(gpio_num_t gpio_num, bool state);

#endif

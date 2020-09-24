#ifndef GPIO_BINDINGS
#define GPIO_BINDINGS

#include "driver/gpio.h"
#include "driver/dac.h"

#define CONTROLLER_STATUS_LED  GPIO_NUM_2

#define LEFT_TRACK             GPIO_NUM_18
#define RIGHT_TRACK            GPIO_NUM_19

#define SPEAKER                DAC_CHANNEL_1   // GPIO Pin 25

#endif

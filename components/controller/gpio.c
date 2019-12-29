#include "gpio.h"

void controller_init_gpio(gpio_num_t gpio_num) {
  gpio_pad_select_gpio(gpio_num);
  ESP_ERROR_CHECK(gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT));
}

void controller_set_gpio(gpio_num_t gpio_num, bool state) {
  ESP_ERROR_CHECK(gpio_set_level(gpio_num, state ? 1 : 0));
}

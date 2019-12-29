#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include <stdio.h>

#include "controller.h"
#include "excavator.h"
#include "gpio_bindings.h"
#include "mac_address.h"

// #define START_LED_GPIO GPIO_NUM_5
// #define SERVO_0_PIN GPIO_NUM_13
// #define SERVO_1_PIN GPIO_NUM_12
//
// #define SERVO_MIN_PWM 600
// #define SERVO_MAX_PWM 2360
// #define SERVO_NEUTRAL_PWM 1500
//
// #define PS3_ANALOG_MIN -128
// #define PS3_ANALOG_MAX 127
//
// /////
// const int frequency = 50;  // Hz
// const int bit_resolution = 15;
//
// int get_duty(int pulse) {
//     double pulse_100_percent = 1e6 / frequency;
//     double duty_100_percent = 1 << bit_resolution;
//
//     double progress = ((double)pulse) / pulse_100_percent;
//     double duty = progress * duty_100_percent;
//
//     return (int)duty;
// }
//
//
// void set_up_servo(int channel, int pin) {
//     // configure channel
//     ledc_channel_config_t channel_config = {
//         .channel = channel,
//         .duty = get_duty(SERVO_NEUTRAL_PWM),
//         .gpio_num = pin,
//         .intr_type = LEDC_INTR_DISABLE,
//         .speed_mode = LEDC_HIGH_SPEED_MODE,
//         .timer_sel = LEDC_TIMER_0
//     };
//     ledc_channel_config(&channel_config);
// }
//
//
// void set_up_servos() {
//     // configure timer
//     ledc_timer_config_t timer_config = {
//         .duty_resolution = bit_resolution,
//         .freq_hz = frequency,
//         .speed_mode = LEDC_HIGH_SPEED_MODE,
//         .timer_num = LEDC_TIMER_0
//     };
//     ledc_timer_config(&timer_config);
//
//     // configure servos
//     set_up_servo(0, SERVO_0_PIN);
//     set_up_servo(1, SERVO_1_PIN);
// }
//
//
// // Supply a value between 127 and -128
// void set_pulse(int channel, int8_t val) {
//     int ps3_range = PS3_ANALOG_MAX - PS3_ANALOG_MIN;
//     int servo_range = SERVO_MAX_PWM - SERVO_MIN_PWM;
//     int pulse = (((val - PS3_ANALOG_MIN) * servo_range) / ps3_range) +
//     SERVO_MIN_PWM;
//
//     ledc_set_duty(LEDC_HIGH_SPEED_MODE, channel, get_duty(pulse));
//     ledc_update_duty(LEDC_HIGH_SPEED_MODE, channel);
// }
// /////
//
// bool isStarted = false;
//
// void toggleStart(void) {
//   isStarted = !isStarted;
//   gpio_set_level(START_LED_GPIO, isStarted ? 1 : 0);
// }
//
// int8_t prev = 0;
// void controller_event_cb(ps3_t ps3, ps3_event_t event) {
//  if(event.button_up.start) {
//    toggleStart();
//  }
//
//  // Don't process the rest of the events when we haven't started yet.
//  if(!isStarted) {
//    return;
//  }
//
//  if(ps3.analog.stick.lx) {
//    set_pulse(0, ps3.analog.stick.lx);
//  }
//
//  if(ps3.analog.stick.ly) {
//    set_pulse(1, ps3.analog.stick.ly);
//  }
//}

// void init_gpio(void)
// {
//   gpio_pad_select_gpio(START_LED_GPIO);
//   gpio_set_direction(START_LED_GPIO, GPIO_MODE_OUTPUT);
// }

void app_main(void) {
  // Initialize NVS flash, it is used by the PS3 library
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  // init_gpio();

  // set_up_servos();

  Excavator *ex = ExcavatorCreate();

  mac_address_print_bt_addr();

  controller_init(GPIO_CONTROLLER_STATUS_LED);
}

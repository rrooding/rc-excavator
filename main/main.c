#include <stdio.h>
#include "ps3.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define STATUS_LED_GPIO GPIO_NUM_2
#define START_LED_GPIO GPIO_NUM_5
#define SERVO_0_PIN GPIO_NUM_13
#define SERVO_1_PIN GPIO_NUM_12

#define SERVO_MIN_PWM 600
#define SERVO_MAX_PWM 2360
#define SERVO_NEUTRAL_PWM 1500

#define PS3_ANALOG_MIN -128
#define PS3_ANALOG_MAX 127

/////
const int frequency = 50;  // Hz
const int bit_resolution = 15;

int get_duty(int pulse) {
    double pulse_100_percent = 1e6 / frequency;
    double duty_100_percent = 1 << bit_resolution;

    double progress = ((double)pulse) / pulse_100_percent;
    double duty = progress * duty_100_percent;

    return (int)duty;
}


void set_up_servo(int channel, int pin) {
    // configure channel
    ledc_channel_config_t channel_config = {
        .channel = channel,
        .duty = get_duty(SERVO_NEUTRAL_PWM),
        .gpio_num = pin,
        .intr_type = LEDC_INTR_DISABLE,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0
    };
    ledc_channel_config(&channel_config);
}


void set_up_servos() {
    // configure timer
    ledc_timer_config_t timer_config = {
        .duty_resolution = bit_resolution,
        .freq_hz = frequency,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0
    };
    ledc_timer_config(&timer_config);

    // configure servos
    set_up_servo(0, SERVO_0_PIN);
    set_up_servo(1, SERVO_1_PIN);
}


// Supply a value between 127 and -128
void set_pulse(int channel, int8_t val) {
    int ps3_range = PS3_ANALOG_MAX - PS3_ANALOG_MIN;
    int servo_range = SERVO_MAX_PWM - SERVO_MIN_PWM;
    int pulse = (((val - PS3_ANALOG_MIN) * servo_range) / ps3_range) + SERVO_MIN_PWM;

    ledc_set_duty(LEDC_HIGH_SPEED_MODE, channel, get_duty(pulse));
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, channel);
}
/////

bool isStarted = false;

void toggleStart(void) {
  isStarted = !isStarted;
  gpio_set_level(START_LED_GPIO, isStarted ? 1 : 0);
}

int8_t prev = 0;
void controller_event_cb(ps3_t ps3, ps3_event_t event) {
  if(event.button_up.start) {
    toggleStart();
  }

  // Don't process the rest of the events when we haven't started yet.
  if(!isStarted) {
    return;
  }

  if(ps3.analog.stick.lx) {
    set_pulse(0, ps3.analog.stick.lx);
  }

  if(ps3.analog.stick.ly) {
    set_pulse(1, ps3.analog.stick.ly);
  }
}

void init_gpio(void)
{
  gpio_pad_select_gpio(STATUS_LED_GPIO);
  gpio_set_direction(STATUS_LED_GPIO, GPIO_MODE_OUTPUT);

  gpio_pad_select_gpio(START_LED_GPIO);
  gpio_set_direction(START_LED_GPIO, GPIO_MODE_OUTPUT);
}

void app_main(void)
{
  // Initialize NVS flash, it is used by the PS3 library
  esp_err_t ret = nvs_flash_init();
  if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  init_gpio();

  set_up_servos();

  // Get base MAC address
  uint8_t mac_addr[8] = {0};
  ret = ESP_OK;

  ret = esp_efuse_mac_get_default(mac_addr);
  if (ret != ESP_OK) {
    printf("ERROR: Failed to get base MAC address from BLK0\n");
    abort();
  }
  else {
    char macStr[19];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
         mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5] + 2);
    printf("Connect PS3 controller to master %s\n", macStr);
  }

  ps3SetEventCallback(controller_event_cb);

  ps3Init();

  printf("Waiting for PS3 controller\n");
  bool blink = false;
  while (!ps3IsConnected()){
      gpio_set_level(STATUS_LED_GPIO, blink ? 1 : 0);
      blink = !blink;
      vTaskDelay(100 / portTICK_PERIOD_MS);
  }

  // 250ms delay after connecting is necessary to allow the channel to clear
  // before pushing the LED configuration.
  vTaskDelay(250 / portTICK_PERIOD_MS);
  ps3SetLed(1);
  vTaskDelay(250 / portTICK_PERIOD_MS);
  ps3Enable();
  gpio_set_level(STATUS_LED_GPIO, 1);
  printf("PS3 controller connected\n");
}

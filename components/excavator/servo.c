#include "driver/ledc.h"
#include "esp_log.h"
#include "servo_internal.h"

#define TAG "EX_SERVO"

static const uint8_t frequency = 50;
static const uint8_t bit_resolution = 15;

static uint8_t nextChannel = 0;
static bool servosInitialized = false;

// MG996R Servo configuration
static servo_pwm_t mg996rPWM = {
  .min = 1000,
  .center = 1500,
  .max = 2000,
  .range = 2000 - 1000
};

static ledc_timer_config_t mg996rTimer = {
  .duty_resolution = bit_resolution,
  .freq_hz = frequency,
  .speed_mode = LEDC_HIGH_SPEED_MODE,
  .timer_num = LEDC_TIMER_0
};


// Model
Servo *ServoCreate(gpio_num_t gpio_num) {
  Servo *newServo = malloc(sizeof(Servo));

  ServoInitialize(newServo, gpio_num);

  return newServo;
}

void ServoFree(Servo *this) { free(this); }

void ServoInitialize(Servo *this, gpio_num_t gpio_num) {
  if(!servosInitialized) { ServoInitializeGlobalState(); }

  // Configure GPIO pin
  gpio_pad_select_gpio(gpio_num);
  ESP_ERROR_CHECK(gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT));

  this->channel = nextChannel++;
  ESP_LOGI(TAG, "Configuring servo on channel %i, %p", this->channel, &this);

  ledc_channel_config_t channel_config = {
    .channel = this->channel,
    .duty = ServoGetDuty(mg996rPWM.center),
    .gpio_num = gpio_num,
    .intr_type = LEDC_INTR_DISABLE,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .timer_sel = LEDC_TIMER_0
  };
  ledc_channel_config(&channel_config);
}

void ServoInitializeGlobalState() {
  ESP_LOGI(TAG, "Configuring servo timers");

  ledc_timer_config(&mg996rTimer);
  servosInitialized = true;
}

void ServoSetSpeed(Servo *this, int8_t speed) {
  int input_range = 128 - -127;
  int pulse = (((speed - -127) * mg996rPWM.range) / input_range) + mg996rPWM.min;

  ESP_LOGI(TAG, "Setting speed %i=>%i", speed, pulse);
  ledc_set_duty(LEDC_HIGH_SPEED_MODE, this->channel, ServoGetDuty(pulse));
  ledc_update_duty(LEDC_HIGH_SPEED_MODE, this->channel);
}

uint16_t ServoGetDuty(int pulse) {
  double pulse_100_percent = 1e6 / frequency;
  double duty_100_percent = 1 << bit_resolution;

  double progress = ((double)pulse) / pulse_100_percent;
  double duty = progress * duty_100_percent;

  return (uint16_t)duty;
}

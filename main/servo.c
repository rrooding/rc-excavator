
// tmp
#include "driver/ledc.h"

#define SERVO_MIN_PWM 500
#define SERVO_MAX_PWM 2500
#define SERVO_NEUTRAL_PWM 1500

#define PS3_ANALOG_STICK_MIN -128
#define PS3_ANALOG_STICK_MAX 127

#define PS3_ANALOG_BUTTON_MIN -255
#define PS3_ANALOG_BUTTON_MAX 255

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
        .bit_num = bit_resolution,
        .freq_hz = frequency,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0
    };
    ledc_timer_config(&timer_config);

    // configure servos
    set_up_servo(0, 18);
    set_up_servo(1, 19);
    set_up_servo(2, 21);
}

int analog_to_pulse(int val, int min, int max) {
    int ps3_range = max - min;
    int servo_range = SERVO_MAX_PWM - SERVO_MIN_PWM;
    int pulse = (((val - min) * servo_range) / ps3_range) +
    SERVO_MIN_PWM;

    return pulse;
}

int stick_to_pulse(int val) {
  return analog_to_pulse(val, PS3_ANALOG_STICK_MIN, PS3_ANALOG_STICK_MAX);
}

int button_to_pulse(int val) {
  return analog_to_pulse(val, PS3_ANALOG_BUTTON_MIN, PS3_ANALOG_BUTTON_MAX);
}

void set_pulse(int channel, int pulse) {
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, channel, get_duty(pulse));
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, channel);
}

controller_init_gpio(GPIO_NUM_18);
controller_init_gpio(GPIO_NUM_19);
controller_init_gpio(GPIO_NUM_21);
set_up_servos();

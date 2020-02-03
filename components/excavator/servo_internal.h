#ifndef EXCAVATOR_SERVO_INTERNAL
#define EXCAVATOR_SERVO_INTERNAL

#include "driver/gpio.h"
#include "servo.h"

typedef struct {
  uint16_t min;
  uint16_t center;
  uint16_t max;
  uint16_t range;
} servo_pwm_t;

struct Servo {
  uint8_t channel;
};

void ServoInitialize(Servo *this, gpio_num_t gpio_num);
void ServoInitializeGlobalState();

uint16_t ServoGetDuty(int pulse);

#endif

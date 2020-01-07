#include "esp_log.h"
#include "servo_internal.h"

#define TAG "EX_SERVO"

Servo *ServoCreate() {
  Servo *newServo = malloc(sizeof(Servo));

  ServoInitialize(newServo);

  return newServo;
}

void ServoFree(Servo *this) { free(this); }

void ServoInitialize(Servo *this) {

}

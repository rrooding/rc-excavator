#ifndef EXCEVATOR_SERVO
#define EXCEVATOR_SERVO

typedef struct Servo Servo;

Servo *ServoCreate();
void ServoFree(Servo *this);

void ServoSetSpeed(Servo *this, int8_t speed);

#endif

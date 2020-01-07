#ifndef EXCEVATOR_SERVO
#define EXCEVATOR_SERVO

typedef struct Servo Servo;

Servo *ServoCreate();
void ServoFree(Servo *this);

#endif

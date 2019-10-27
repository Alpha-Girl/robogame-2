#include <Servo.h>
#include <Arduino.h>
#define sattach1 9
#define sattach2 10
#define sattach3 11
#define sattach4 12
#define vcc A14
#define sm_pls A6
#define sm_dir A5
#define sm_ena A4

//dir:1 is clockwise, 0 is anticlockwise.rotate 90 degrees
void stepped_motor(int dir);
void sm_adapt();

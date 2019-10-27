#define forward 0
#define leftward 1
#define backward 2
#define rightward 3

#define f0 22
#define f1 23
#define f2 24
#define f3 25
#define f4 26

#define l0 27
#define l1 28
#define l2 29
#define l3 30
#define l4 31

#define b0 32
#define b1 33
#define b2 34
#define b3 35
#define b4 36

#define r0 37
#define r1 38
#define r2 39
#define r3 40
#define r4 41

#include <Arduino.h>

typedef boolean _01;
typedef struct
{
    byte sn;
    byte pin0, pin1, pin2, pin3, pin4;
} infrare_module;


int8_t Check_Err_Num(uint8_t dir, uint8_t ModelNo);
void adapt(int8_t f_err_no, int8_t b_err_no, int16_t motion_para[], uint8_t dir);
void trailing(uint8_t dir);
void reset(int16_t default_rotating_speed[]);
void stay_adapt(uint8_t dir, uint8_t previous_dir);
int8_t for_stay_adapt(uint8_t ModuleNo);
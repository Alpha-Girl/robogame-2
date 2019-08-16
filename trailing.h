#ifndef _trailing
#define _trailing

#include <Arduino.h>

#define forward 0
#define leftward 1
#define backward 2
#define rightward 3

#define f0 40
#define f1 41
#define f2 42

#define b0 43
#define b1 44
#define b2 45

#define l0 46
#define l1 47
#define l2 48

#define r0 49
#define r1 50
#define r2 51

typedef boolean _01;
typedef struct
{
    byte sn;
    byte pin0, pin1, pin2;
} infrare_model;

int8_t Check_Err_Num(uint8_t dir, uint8_t ModelNo);
void adapt(int8_t f_err_no, int8_t b_err_no, int16_t motion_para[], uint8_t dir);
//void adjust_rotating_speed(int16_t rotating_speed[]);//相关函数已移动到move.cpp中
void trailing(uint8_t dir);

#endif

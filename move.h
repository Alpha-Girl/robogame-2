#ifndef _move
#define _move

#include <Arduino.h>

#define en_pwm_1 2
#define en_pwm_2 3
#define en_pwm_3 4
#define en_pwm_4 5

#define rpwm_en_1 22
#define lpwm_en_1 23

#define rpwm_en_2 24
#define lpwm_en_2 25

#define rpwm_en_3 26
#define lpwm_en_3 27

#define rpwm_en_4 28
#define lpwm_en_4 29

void calc_rotating_speed(int16_t Motion_Parameters[], int16_t rotating_speed[]);
void set_pin(int16_t rotating_speed[]);
void rotate_car(int16_t degree);
void default_pin_set();
void stop();

#endif

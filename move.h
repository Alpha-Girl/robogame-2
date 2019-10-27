#include <Arduino.h>
#define en_pwm_1 2
#define en_pwm_2 3
#define en_pwm_3 6
#define en_pwm_4 5

#define rpwm_en_1 42
#define lpwm_en_1 43

#define rpwm_en_2 44
#define lpwm_en_2 45

#define rpwm_en_3 46
#define lpwm_en_3 47

#define rpwm_en_4 48
#define lpwm_en_4 49

void calc_rotating_speed(int16_t Motion_Parameters[], int16_t rotating_speed[]);
void set_pin(int16_t rotating_speed[],bool xa);
//void rotate_car(int16_t degree);
void rotate_car(bool clockwise_, uint8_t end_dir);
void default_pin_set();
void stop();

#include <Arduino.h>
#include "move.h"

const int16_t Inv_Kinematic_Mat[4][3] = {{1, 1, 1}, {-1, 1, -1}, {1, 1, -1}, {-1, 1, 1}}; //逆运动学矩阵

void calc_rotating_speed(int16_t Motion_Parameters[], int16_t rotating_speed[])
{
    for (byte i = 0; i < 4; i++)
    {
        int16_t temp = 0;
        for (byte j = 0; j < 3; j++)
            temp += Inv_Kinematic_Mat[i][j] * Motion_Parameters[j];
        rotating_speed[i] = temp;
    }
    int max = 0;
    for (byte i = 0; i < 4; i++)
        if (abs(rotating_speed[i]) > max)
            max = abs(rotating_speed[i]);
    for (byte i = 0; i < 4; i++)
        rotating_speed[i] = rotating_speed[i] / max * 255;
}

void set_pin(int16_t rotating_speed[])
{
    byte en_pwm[4] = {2, 3, 4, 5};
    byte pwm_en[8] = {22, 23, 24, 25, 26, 27, 28, 29};
    default_pin_set();
    for (byte i = 0; i < 4; i++)
    {
        if (rotating_speed[i] < 0)
        {
            analogWrite(en_pwm[i], -rotating_speed[i]);
            digitalWrite(pwm_en[2 * i + 1], HIGH);
            digitalWrite(pwm_en[2 * i], LOW);
        }
        else
        {
            analogWrite(en_pwm[i], rotating_speed[i]);
            digitalWrite(pwm_en[2 * i], HIGH);
            digitalWrite(pwm_en[2 * i + 1], LOW);
        }
    }
}

void rotate_car(int16_t degree)
{
    int16_t clockwise[4] = {-255, 255, 255, -255};
    int16_t anticlockwise[4] = {255, -255, -255, 255};
    if (degree > 0)
    {
        set_pin(clockwise);
        delay(degree / 360 * 1000);
        stop();
    }
    else
    {
        set_pin(anticlockwise);
        delay(degree / 360 * 1000);
        stop();
    }
}

void default_pin_set()
{
    digitalWrite(rpwm_en_1, LOW);
    digitalWrite(lpwm_en_1, LOW);
    digitalWrite(rpwm_en_2, LOW);
    digitalWrite(lpwm_en_2, LOW);
    digitalWrite(rpwm_en_3, LOW);
    digitalWrite(lpwm_en_3, LOW);
    digitalWrite(rpwm_en_4, LOW);
    digitalWrite(lpwm_en_4, LOW);
}

void stop()
{
    default_pin_set();
}
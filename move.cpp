#include "move.h"
#include "trailing.h"

void rough_stop(bool clockwise_);
//const int16_t Inv_Kinematic_Mat[4][3] = {{1, 1, 1}, {-1, 1, -1}, {1, 1, -1}, {-1, 1, 1}}; //逆运动学矩阵
const int16_t Inv_Kinematic_Mat[4][3] = {{-1, 1, 1}, {1, 1, -1}, {-1, 1, -1}, {1, 1, 1}}; //逆运动学矩阵
infrare_module infmodule2[4] = {{0, f0, f1, f2, f3, f4}, {1, l0, l1, l2, l3, l4}, {2, b0, b1, b2, b3, b4}, {3, r0, r1, r2, r3, r4}};

void calc_rotating_speed(int16_t Motion_Parameters[], int16_t rotating_speed[])
{
    for (byte i = 0; i < 4; i++)
    {
        int16_t temp = 0;
        for (byte j = 0; j < 3; j++)
            temp += Inv_Kinematic_Mat[i][j] * Motion_Parameters[j];
        rotating_speed[i] = temp;
    }
    int16_t max = 0;
    for (byte i = 0; i < 4; i++)
        if (abs(rotating_speed[i]) > max)
            max = abs(rotating_speed[i]);
    if (max > 255)
        for (byte i = 0; i < 4; i++)
            rotating_speed[i] = (int16_t)(((float)rotating_speed[i] / (float)max) * 255.0);
}

//xa:=1 left,0 right
void set_pin(int16_t rotating_speed[], bool xa)
{

    byte en_pwm1[4] = {2, 3, 6, 5};
    byte pwm_en1[8] = {42, 43, 44, 45, 46, 47, 48, 49};
    //byte en_pwm2[4] = {5, 6, 3, 2};
    //byte pwm_en2[8] = {48, 49, 46, 47, 44, 45, 42, 43};
    default_pin_set();
    if (1 || xa)
        for (byte i = 0; i < 4; i++)
        {
            if (rotating_speed[i] < 0)
            {
                analogWrite(en_pwm1[i], -rotating_speed[i]);
                digitalWrite(pwm_en1[2 * i + 1], HIGH);
                digitalWrite(pwm_en1[2 * i], LOW);
            }
            else
            {
                analogWrite(en_pwm1[i], rotating_speed[i]);
                digitalWrite(pwm_en1[2 * i], HIGH);
                digitalWrite(pwm_en1[2 * i + 1], LOW);
            }
        }
    else
        for (byte i = 2;; i = (i + 1) % 4)
        {
            if (rotating_speed[i] < 0)
            {
                analogWrite(en_pwm1[i], -rotating_speed[i]);
                digitalWrite(pwm_en1[2 * i + 1], HIGH);
                digitalWrite(pwm_en1[2 * i], LOW);
            }
            else
            {
                analogWrite(en_pwm1[i], rotating_speed[i]);
                digitalWrite(pwm_en1[2 * i], HIGH);
                digitalWrite(pwm_en1[2 * i + 1], LOW);
            }
            if (i == 1)
                break;
        }
}

void rotate_car(bool clockwise_, uint8_t end_dir)
{
    //int16_t clockwise[4] = {-115, 85, 115, -85};
    //int16_t anticlockwise[4] = {85, -115, -85, 115};

    int16_t clockwise[4] = {-130, 130, 130, -130};
    int16_t anticlockwise[4] = {130, -130, -130, 130};

    if (clockwise_)
    {
        set_pin(clockwise, 1);
        delay(250);
        while (1)
        {
            if ((end_dir < 2 ? digitalRead(infmodule2[end_dir].pin3) == 1 : digitalRead(infmodule2[end_dir].pin1) == 1))
            {
                rough_stop(clockwise_);
                break;
            }
        }
    }
    else
    {
        set_pin(anticlockwise, 1);
        //delay((long)(((float)(-degree)) / 360.0 * 5000.0));
        delay(250);
        while (1)
        {
            if (end_dir == 2 && digitalRead(infmodule2[end_dir].pin4) == 1)
            {
                rough_stop(clockwise_);
                break;
            }
            if ((end_dir < 2 ? digitalRead(infmodule2[end_dir].pin1) == 1 : digitalRead(infmodule2[end_dir].pin3) == 1))
            {
                rough_stop(clockwise_);
                break;
            }
        }
    }
}

void default_pin_set()
{
    //digitalWrite(en_pwm_1, LOW);
    //digitalWrite(en_pwm_2, LOW);
    //digitalWrite(en_pwm_3, LOW);
    //digitalWrite(en_pwm_4, LOW);
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

void rough_stop(bool clockwise_)
{
    if (clockwise_)
    {

        int16_t rs[4] = {-5, 5, 5, -5};
        set_pin(rs, 1);
    }
    else
    {
        int16_t rs[4] = {5, -5, -5, 5};

        set_pin(rs, 1);
    }
    const long time = millis();
    while (millis() <= time + 100)
        ;
    stop();
}
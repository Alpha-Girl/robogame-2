#include <Arduino.h>
#include "trailing.h"
#include "move.h"

infrare_model infmodel[4] = {{0, f0, f1, f2}, {1, l0, l1, l2}, {2, b0, b1, b2}, {3, r0, r1, r2}};

int8_t Check_Err_Num(uint8_t dir, uint8_t ModelNo)
{
    if (dir == 0 || dir == 1)
    {
        _01 check0 = digitalRead(infmodel[ModelNo].pin0);
        _01 check1 = digitalRead(infmodel[ModelNo].pin1);
        _01 check2 = digitalRead(infmodel[ModelNo].pin2);
        if (check0 && check2)
            return 0;
        if (check1 && !check2)
            return 1;
        if (check0 && !check1)
            return 2;
        if (!check0 && check1)
            return -1;
        if (!check1 && check2)
            return -2;
        if (!check0 && !check2)
            return -3;
    }
    else if (dir == 2 || dir == 3)
    {
        _01 check0 = digitalRead(infmodel[ModelNo].pin2);
        _01 check1 = digitalRead(infmodel[ModelNo].pin1);
        _01 check2 = digitalRead(infmodel[ModelNo].pin0);
        if (check0 && check2)
            return 0;
        if (check1 && !check2)
            return 1;
        if (check0 && !check1)
            return 2;
        if (!check0 && check1)
            return -1;
        if (!check1 && check2)
            return -2;
        if (!check0 && !check2)
            return -3;
    }
}

void adapt(int8_t f_err_no, int8_t b_err_no, int16_t motion_para[], uint8_t dir)
{
    int16_t rotating_speed[4] = {0, 0, 0, 0};
    int8_t err_no = f_err_no + b_err_no;
    if (dir == 0 || dir == 2)
    {
        if (f_err_no > b_err_no) //逆时针旋转，平移方向依情况而定
        {
            if (err_no > 0)
                motion_para[0] = (dir == 0 ? -1 : 1) * -30;
            else if (err_no < 0)
                motion_para[0] = (dir == 0 ? -1 : 1) * 30;
            motion_para[2] = 45;
            calc_rotating_speed(motion_para, rotating_speed);
            //adjust_rotating_speed(rotating_speed);
            set_pin(rotating_speed);
        }

        else if (f_err_no == b_err_no && f_err_no != -3) //只需平移
        {
            motion_para[0] = (dir == 0 ? 1 : -1) * (f_err_no > 0 ? -30 : 30);
            calc_rotating_speed(motion_para, rotating_speed);
            //adjust_rotating_speed(rotating_speed);
            set_pin(rotating_speed);
        }
        else if (f_err_no == -3 || b_err_no == -3)
        {
            if (f_err_no == -3 && b_err_no == -3)
                stop();
            else if (f_err_no == -3)
            {
                if (b_err_no > 0)
                    motion_para[0] = (dir == 0 ? -1 : 1) * 30;
                else if (b_err_no < 0)
                    motion_para[0] = (dir == 0 ? -1 : 1) * -30;
                calc_rotating_speed(motion_para, rotating_speed);
                //adjust_rotating_speed(rotating_speed);
                set_pin(rotating_speed);
            }
            else
            {
                if (f_err_no > 0)
                    motion_para[0] = (dir == 0 ? -1 : 1) * 30;
                else if (f_err_no < 0)
                    motion_para[0] = (dir == 0 ? -1 : 1) * -30;
                calc_rotating_speed(motion_para, rotating_speed);
                //adjust_rotating_speed(rotating_speed);
                set_pin(rotating_speed);
            }
        }
        else //旋转情况与true相反
        {
            if (err_no > 0)
                motion_para[0] = (dir == 0 ? -1 : 1) * -30;
            else if (err_no < 0)
                motion_para[0] = (dir == 0 ? -1 : 1) * 30;
            motion_para[2] = -45;
            calc_rotating_speed(motion_para, rotating_speed);
            //adjust_rotating_speed(rotating_speed);
            set_pin(rotating_speed);
        }
    }
    else
    {
        if (f_err_no > b_err_no) //逆时针旋转，平移方向依情况而定
        {
            if (err_no > 0)
                motion_para[1] = (dir == 1 ? -1 : 1) * -30;
            else if (err_no < 0)
                motion_para[1] = (dir == 1 ? -1 : 1) * 30;
            motion_para[2] = 45;
            calc_rotating_speed(motion_para, rotating_speed);
            //adjust_rotating_speed(rotating_speed);
            set_pin(rotating_speed);
        }

        else if (f_err_no == b_err_no && f_err_no != -3) //只需平移
        {
            motion_para[1] = (dir == 1 ? 1 : -1) * (f_err_no > 0 ? -30 : 30);
            calc_rotating_speed(motion_para, rotating_speed);
            //adjust_rotating_speed(rotating_speed);
            set_pin(rotating_speed);
        }
        else if (f_err_no == -3 || b_err_no == -3)
        {
            if (f_err_no == -3 && b_err_no == -3)
                stop();
            else if (f_err_no == -3)
            {
                if (b_err_no > 0)
                    motion_para[1] = (dir == 1 ? -1 : 1) * 30;
                else if (b_err_no < 0)
                    motion_para[1] = (dir == 1 ? -1 : 1) * -30;
                calc_rotating_speed(motion_para, rotating_speed);
                // adjust_rotating_speed(rotating_speed);
                set_pin(rotating_speed);
            }
            else
            {
                if (f_err_no > 0)
                    motion_para[1] = (dir == 1 ? -1 : 1) * 30;
                else if (f_err_no < 0)
                    motion_para[1] = (dir == 1 ? -1 : 1) * -30;
                calc_rotating_speed(motion_para, rotating_speed);
                //adjust_rotating_speed(rotating_speed);
                set_pin(rotating_speed);
            }
        }
        else //旋转情况与true相反
        {
            if (err_no > 0)
                motion_para[1] = (dir == 1 ? -1 : 1) * -30;
            else if (err_no < 0)
                motion_para[1] = (dir == 1 ? -1 : 1) * 30;
            motion_para[2] = -45;
            calc_rotating_speed(motion_para, rotating_speed);
            //adjust_rotating_speed(rotating_speed);
            set_pin(rotating_speed);
        }
    }
}

/* void adjust_rotating_speed(int16_t rotating_speed[])
{
    int max = 0;
    for (byte i = 0; i < 4; i++)
        if (abs(rotating_speed[i]) > max)
            max = abs(rotating_speed[i]);
    for (byte i = 0; i < 4; i++)
        rotating_speed[i] = rotating_speed[i] / max * 255;
}*/
//相关函数已移动到move.cpp中

void trailing(uint8_t dir)
{
    int16_t default_rotating_speed[4][4] =
        {{255, 255, 255, 255}, {-255, 255, -255, 255}, {-255, -255, -255, -255}, {255, -255, 255, -255}};
    int16_t motion_para[4][3] =
        {{0, 255, 0}, {-255, 0, 0}, {0, -255, 0}, {255, 0, 0}};
    int8_t f_err_no = Check_Err_Num(dir, dir);
    int8_t b_err_no = Check_Err_Num(dir, (dir + 2) % 4);
    if (f_err_no == 0 && b_err_no == 0)
        reset(default_rotating_speed[dir]);
    else
        adapt(f_err_no, b_err_no, motion_para[dir], dir);
}

void reset(int16_t default_rotating_speed[])
{
    set_pin(default_rotating_speed);
}
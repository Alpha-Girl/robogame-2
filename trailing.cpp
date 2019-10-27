#include "trailing.h"
#include "move.h"

infrare_module infmodule[4] = {{0, f0, f1, f2, f3, f4}, {1, l0, l1, l2, l3, l4}, {2, b0, b1, b2, b3, b4}, {3, r0, r1, r2, r3, r4}};

int8_t Check_Err_Num(uint8_t dir, uint8_t ModuleNo)
{
    if (dir == 0 || dir == 1)
    {
        _01 check1 = digitalRead(infmodule[ModuleNo].pin1);
        _01 check2 = digitalRead(infmodule[ModuleNo].pin2);
        _01 check3 = digitalRead(infmodule[ModuleNo].pin3);
        if (check1 && check3)
            return 0;
        if (check2 && !check3)
            return 1;
        if (check1 && !check2)
            return 2;
        if (!check1 && check2)
            return -1;
        if (!check2 && check3)
            return -2;
        if (!check1 && !check3)
            return -3;
    }
    else if (dir == 2 || dir == 3)
    {
        _01 check1 = digitalRead(infmodule[ModuleNo].pin3);
        _01 check2 = digitalRead(infmodule[ModuleNo].pin2);
        _01 check3 = digitalRead(infmodule[ModuleNo].pin1);
        if (check1 && check3)
            return 0;
        if (check2 && !check3)
            return 1;
        if (check1 && !check2)
            return 2;
        if (!check1 && check2)
            return -1;
        if (!check2 && check3)
            return -2;
        if (!check1 && !check3)
            return -3;
    }
}

int8_t for_stay_adapt(uint8_t ModuleNo)
{
    bool status_of_line[5] = {digitalRead(infmodule[ModuleNo].pin0),
                              digitalRead(infmodule[ModuleNo].pin1),
                              digitalRead(infmodule[ModuleNo].pin2),
                              digitalRead(infmodule[ModuleNo].pin3),
                              digitalRead(infmodule[ModuleNo].pin4)};
    int sum = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += status_of_line[i];
    }

    //===========================
    /*Serial.print("sum=");
    Serial.println(sum);
    for (int i = 0; i < 5; i++)
        Serial.print(status_of_line[i]);
    Serial.println();*/
    //===========================

    switch (sum)
    {
    case 4:
        return 0;
        break;
    case 3:
        if (status_of_line[3] == 0)
            return 1;
        else if (status_of_line[1] == 0)
            return -1;
        else
            return 0;
        break;
    case 2:
        if (status_of_line[3] == 0)
            return 2;
        else if (status_of_line[1] == 0)
            return -2;
        break;
    case 1:
        if (status_of_line[4] == 1)
            return -3;
        else if (status_of_line[0] == 1)
            return 3;
        break;
    case 0:
        return -4;
    }
}

void stay_adapt(uint8_t dir, uint8_t pre_dir)
{
    int8_t err0 = for_stay_adapt(dir);
    //int8_t err1 = for_stay_adapt(1);
    //int8_t err2 = for_stay_adapt(2);
    //int8_t err3 = for_stay_adapt(3);
    int motion[3] = {0, 0, 0};
    if (dir == 0 || dir == 2)
    {

        if (err0 > 0)
            motion[0] = -80;
        else if (err0 < 0)
            motion[0] = 80;
    }
    else if (dir == 1 || dir == 3)
    {
        if (err0 > 0)
            motion[1] = -80;
        else if (err0 < 0)
            motion[1] = 80;
    }
    int16_t rs[4];
    calc_rotating_speed(motion, rs);
    set_pin(rs, 1);
    static int s_err_stay;
    do
    {
        s_err_stay = for_stay_adapt(dir);
    } while (s_err_stay != 0);
    stop();
}

void adapt(int8_t f_err_no, int8_t b_err_no, int16_t motion_para[], uint8_t dir)
{
    int16_t rotating_speed[4] = {0, 0, 0, 0};
    int8_t err_no = f_err_no + b_err_no;
    if (dir == 0 || dir == 2)
    {
        if (f_err_no != -3 && b_err_no != -3)
        {
            if (f_err_no > b_err_no) //逆时针旋转，平移方向依情况而定
            {
                if (err_no > 0)
                    motion_para[0] = (dir == 0 ? -125 : 125);
                else if (err_no < 0)
                    motion_para[0] = (dir == 0 ? 125 : -125);
                motion_para[2] = 125;
                calc_rotating_speed(motion_para, rotating_speed);
                set_pin(rotating_speed, 1);
            }
            else if (f_err_no == b_err_no) //只需平移
            {
                if (dir == 0)
                    motion_para[0] = (f_err_no > 0 ? -125 : 125);
                else
                    motion_para[0] = (f_err_no > 0 ? 125 : -125);
                calc_rotating_speed(motion_para, rotating_speed);
                set_pin(rotating_speed, 1);
            }
            else //旋转情况与true相反
            {
                if (err_no > 0)
                    motion_para[0] = (dir == 0 ? -125 : 125);
                else if (err_no < 0)
                    motion_para[0] = (dir == 0 ? 125 : -125);
                motion_para[2] = -125;
                calc_rotating_speed(motion_para, rotating_speed);
                set_pin(rotating_speed, 1);
            }
        }
        else
        {
            if (f_err_no == -3 && b_err_no == -3)
                stop();
            else if (f_err_no == -3)
            {
                if (b_err_no > 0)
                    motion_para[0] = (dir == 0 ? -125 : 125);
                else if (b_err_no < 0)
                    motion_para[0] = (dir == 0 ? 125 : -125);
                calc_rotating_speed(motion_para, rotating_speed);
                set_pin(rotating_speed, 1);
            }
            else
            {
                if (f_err_no > 0)
                    motion_para[0] = (dir == 0 ? -125 : 125);
                else if (f_err_no < 0)
                    motion_para[0] = (dir == 0 ? 125 : -125);
                calc_rotating_speed(motion_para, rotating_speed);
                set_pin(rotating_speed, 1);
            }
        }
    }
    else if (dir == 1 || dir == 3)
    {
        if (f_err_no != -3 && b_err_no != -3)
        {
            if (f_err_no > b_err_no) //逆时针旋转，平移方向依情况而定
            {
                if (err_no > 0)
                    motion_para[1] = (dir == 1 ? -30 : 30);
                else if (err_no < 0)
                    motion_para[1] = (dir == 1 ? 30 : -30);
                motion_para[2] = 30;
                calc_rotating_speed(motion_para, rotating_speed);
                set_pin(rotating_speed, 1);
            }
            else if (f_err_no == b_err_no) //只需平移
            {
                if (dir == 1)
                    motion_para[1] = (f_err_no > 0 ? -30 : 30);
                else
                    motion_para[1] = (f_err_no > 0 ? 30 : -30);
                calc_rotating_speed(motion_para, rotating_speed);
                set_pin(rotating_speed, 1);
            }
            else //旋转情况与true相反
            {
                if (err_no > 0)
                    motion_para[1] = (dir == 1 ? -30 : 30);
                else if (err_no < 0)
                    motion_para[1] = (dir == 1 ? 30 : -30);
                motion_para[2] = -30;
                calc_rotating_speed(motion_para, rotating_speed);
                set_pin(rotating_speed, 1);
            }
        }
        else
        {
            if (f_err_no == -3 && b_err_no == -3)
                stop();
            else if (f_err_no == -3)
            {
                if (b_err_no > 0)
                    motion_para[1] = (dir == 1 ? -0 : 30);
                else if (b_err_no < 0)
                    motion_para[1] = (dir == 1 ? 30 : -30);
                calc_rotating_speed(motion_para, rotating_speed);
                set_pin(rotating_speed, 1);
            }
            else
            {
                if (f_err_no > 0)
                    motion_para[1] = (dir == 1 ? -30 : 30);
                else if (f_err_no < 0)
                    motion_para[1] = (dir == 1 ? 30 : -30);
                calc_rotating_speed(motion_para, rotating_speed);
                set_pin(rotating_speed, 1);
            }
        }
    }
}

void trailing(uint8_t dir)
{
    static bool is_changed_zero = true;
    //int16_t default_rotating_speed[4][4] =
    //    {{255, 255, 255, 255}, {-255, 255, -255, 255}, {-255, -255, -255, -255}, {255, -255, 255, -255}};
    int16_t default_rotating_speed[4][4] =
        {{255, 255, 255, 255}, {255, -255, 255, -255}, {-255, -255, -255, -255}, {-255, 255, -255, 255}};

    int16_t motion_para[4][3] =
        {{0, 255, 0}, {-255, 0, 0}, {0, -255, 0}, {255, 0, 0}};
    int8_t f_err_no = Check_Err_Num(dir, dir);
    int8_t b_err_no = Check_Err_Num(dir, (dir + 2) % 4);
    if (f_err_no == 0 && b_err_no == 0)
    {
        if (is_changed_zero)
        {
            set_pin(default_rotating_speed[dir], 1);
            is_changed_zero = false;
        }
    }
    else
    {
        adapt(f_err_no, b_err_no, motion_para[dir], dir);
        is_changed_zero = true;
    }
}

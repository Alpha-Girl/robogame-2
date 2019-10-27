#define leftstage
//#define rightstage
//#define testing
//#define testcode

#include "move.h"
#include "trailing.h"
#include "Rfid_D_Think_M50.h"
#include "ultrasonic.h"
#include "s_moto__catch.h"
#define BLOCK_NO 0

//0:top cube
//1:middle cube
//2:down cube
//3:rec cube
Servo mys[4];

Servo anti_sm;

typedef struct
{
    uint8_t data[20];
    //uint8_t data[20];
    uint8_t top = 0;
} rfid_read_already;

Rfid_D_Think_M50 rfid(Serial1);
uint8_t last_status;
rfid_read_already RFIDRA;
/*int16_t rsr[4] = {171, -171, 183, -183};
int16_t rsl[4] = {-173, 173, -170, 170};*/
int16_t rsf[4] = {170, 170, 170, 170};
int16_t rsb[4] = {-100, -100, -100, -100};
//int16_t rsr[4] = {177, -177, 202, -202};

int16_t rsr[4] = {-175, 175, -175, 175};
int16_t rsl[4] = {170, -170, 170, -170};

int cubecolor[3] = {0}; //= {2, 3, 6};         //= {0};
int boxcolor[6] = {0};  //= {3, 2, 1, 4, 5, 6}; //= {0};

void setup()
{
    Serial.begin(9600);
    Serial.print(1);
    int inputflag = 0;
    while (1)
    {
        if (Serial.available() > 0)
        {
            delay(1000);
            while (inputflag < 9)
            {
                if (inputflag < 6)
                {
                    boxcolor[5 - inputflag] = Serial.read() - 48;
                    inputflag++;
                }
                else
                {
                    cubecolor[inputflag - 6] = Serial.read() - 48;
                    inputflag++;
                }
            }
            break;
        }
    }
    delay(1000);
    /*for (int i = 0; i < 9; i++)
    {
        Serial.print(i + 1);
        while (1)
        {
            if (Serial.available() > 0)
            {
                if (i < 6)
                {
                    boxcolor[5 - i] = Serial.read() - 48;
                }
                else
                {
                    cubecolor[i - 6] = Serial.read() - 48;
                }
                while (Serial.available() > 0)
                    Serial.read();
                break;
            }
        }
        delay(100);
    }*/
    /*
#ifdef rightstage
    for (int i = 0; i < 9; i++)
    {
        Serial.print(i + 1);
        while (1)
        {
            if (Serial.available() > 0)
            {
                if (i < 6)
                {
                    boxcolor[i] = Serial.read() - 48;
                }
                else
                {
                    cubecolor[8 - i] = Serial.read() - 48;
                }
                while (Serial.available() > 0)
                    Serial.read();
                break;
            }
        }
        delay(1000);
    }
#endif
*/
    //初始移动设置
    last_status = forward;
    trailing(last_status);

    //红外设置
    for (int i = 22; i <= 41; i++)
        pinMode(i, INPUT);
    pinMode(A8, OUTPUT);
    pinMode(A9, INPUT);
    digitalWrite(A8, HIGH);

    //移动设置
    pinMode(en_pwm_1, OUTPUT);
    pinMode(en_pwm_2, OUTPUT);
    pinMode(en_pwm_3, OUTPUT);
    pinMode(en_pwm_4, OUTPUT);

    pinMode(rpwm_en_1, OUTPUT);
    pinMode(lpwm_en_1, OUTPUT);

    pinMode(rpwm_en_2, OUTPUT);
    pinMode(lpwm_en_2, OUTPUT);

    pinMode(rpwm_en_3, OUTPUT);
    pinMode(lpwm_en_3, OUTPUT);

    pinMode(rpwm_en_4, OUTPUT);
    pinMode(lpwm_en_4, OUTPUT);

    //超声测距设置
    pinMode(trigl, OUTPUT);
    pinMode(echol, INPUT);
    pinMode(trigr, OUTPUT);
    pinMode(echor, INPUT);
    pinMode(A15, OUTPUT);
    digitalWrite(A15, HIGH);

    //RFIDRA设置
    rfid.enableAntenna(rfid.ISO15693);

    //舵机及步进电机设置
    mys[2].attach(sattach1);
    mys[3].attach(sattach2);
    mys[1].attach(sattach3);
    mys[0].attach(sattach4);
    anti_sm.attach(8);
    pinMode(vcc, OUTPUT);
    digitalWrite(vcc, HIGH);
    pinMode(sm_pls, OUTPUT);
    pinMode(sm_dir, OUTPUT);
    pinMode(sm_ena, OUTPUT);
    digitalWrite(sm_ena, HIGH);
    anti_sm.write(120);
    mys[0].write(0);
    mys[1].write(0);
    mys[2].write(0);
    mys[3].write(0);
    /*mys[0].write(180);
    mys[1].write(180);
    mys[2].write(180);
    mys[3].write(180);*/
}

int comparecolor(int cubecolor[], int boxcolor[], int flag)
{
    if (cubecolor[0] == boxcolor[flag])
        return 0;
    else if (cubecolor[1] == boxcolor[flag])
        return 1;
    else if (cubecolor[2] == boxcolor[flag])
        return 2;
    else
        return -1;
}

void put_in(int putorder[], int index)
{
    if (putorder[index] == 0)
    {
        anti_sm.write(60);
        delay(300);
        stepped_motor(1);
        delay(3400);
        mys[putorder[index]].write(0);
        delay(1000);
        stepped_motor(0);
        delay(3400);
        anti_sm.write(120);
    }
    else if (putorder[index] == 1)
    {
        anti_sm.write(60);
        delay(300);
        stepped_motor(0);
        delay(3400);
        mys[putorder[index]].write(0);
        delay(1000);
        stepped_motor(1);
        delay(3400);
        anti_sm.write(120);
    }
    else if (putorder[index] == 2)
    {
        mys[putorder[index]].write(0);
        delay(1000);
    }
}

//stage:=0 left,1 right
void put(int cubecolor[], int boxcolor[], int stage)
{
    int putorder[6];
    int putindex[3];
    int j = 0;
    for (int i = 0; i < 6; i++)
        putorder[i] = comparecolor(cubecolor, boxcolor, i);
    for (int i = 0; i < 6; i++)
        if (putorder[i] != -1)
            putindex[j++] = i;
    j = 0;
    uint8_t rfid_read_now[4] = {0, 0, 0, 0};
    while (1)
    {
        rfid.read(BLOCK_NO, rfid_read_now);
        trailing(forward);
        if (j == 3 && rfid_read_now[0] == 22)
        {
            stop();
            delay(300);
            stage == 0 ? stay_adapt(leftward, leftward) : stay_adapt(rightward, leftward);
            delay(300);
            stage == 0 ? set_pin(rsl, 1) : set_pin(rsr, 1);
            static double usdisput3 = stage == 0 ? ultrasonic_distance(us_left) : ultrasonic_distance(us_right);
            delay(30);
            do
            {
                usdisput3 = stage == 0 ? ultrasonic_distance(us_left) : ultrasonic_distance(us_right);
                delay(30);
            } while (usdisput3 > 5.5);
            stop();
            delay(300);
            stage == 0 ? stay_adapt(leftward, leftward) : stay_adapt(rightward, leftward);
            delay(300);
            anti_sm.write(60);
            delay(300);
            stepped_motor(1);
            delay(3200);
            stepped_motor(1);
            delay(3400);
            mys[3].write(0);
            delay(1000);
            j++;
        }
        else if (rfid_read_now[0] == putindex[j] + 16)
        {
            stop();
            delay(300);
            stage == 0 ? stay_adapt(leftward, leftward) : stay_adapt(rightward, leftward);
            delay(300);
            stage == 0 ? set_pin(rsl, 1) : set_pin(rsr, 1);
            static double usdisput0 = stage == 0 ? ultrasonic_distance(us_left) : ultrasonic_distance(us_right);
            delay(30);
            if (putorder[putindex[j]] == 1)
                do
                {
                    usdisput0 = stage == 0 ? ultrasonic_distance(us_left) : ultrasonic_distance(us_right);
                    delay(30);
                } while (usdisput0 > 12.0);
            else
                do
                {
                    usdisput0 = stage == 0 ? ultrasonic_distance(us_left) : ultrasonic_distance(us_right);
                    delay(30);
                } while (usdisput0 > 17.0);
            stop();
            delay(300);
            stage == 0 ? stay_adapt(leftward, leftward) : stay_adapt(rightward, leftward);
            put_in(putorder, putindex[j]);
            j++;
            stage == 0 ? set_pin(rsr, 1) : set_pin(rsl, 1);
            do
            {
            } while (Check_Err_Num(0, 0) == -3 || Check_Err_Num(0, 2) == -3);
            stop();
            delay(300);
            stay_adapt(forward, forward);
            delay(300);
        }
        delay(50);
    }

    //=================================================

    //true:closed
    //flase:far
    //bool ult_pre_status = true;
    //true:closed
    //flase:far
    //bool ult_status = false;
    /*
    int flag = putindex[0];
    j = 0;
    while (1)
    {
        if ((stage == 0 ? ultrasonic_distance(us_left) : ultrasonic_distance(us_right)) < 30.0)
            ult_status = true;
        else
            ult_status = false;
        //==============================================
        if (!ult_pre_status && ult_status)
            flag++;
        //==============================================
        
        if (j != 3 &&  putindex[j])
        {
            do
            {
                trailing(forward);
            } while (Check_Err_Num(forward, leftward) == -3);
            stop();
            delay(300);
            if (ultrasonic_distance(us_left) > 18.0)
            {
                stage == 0 ? set_pin(rsl, 1) : set_pin(rsr, 1);
                static double usdisput1 = stage == 0 ? ultrasonic_distance(us_left) : ultrasonic_distance(us_right);
                do
                {
                    usdisput1 = stage == 0 ? ultrasonic_distance(us_left) : ultrasonic_distance(us_right);
                    delay(30);
                } while (usdisput1 > 18.0);
            }
            stop();
            delay(300);
            stage == 0 ? stay_adapt(leftward, leftward) : stay_adapt(rightward, leftward);
            delay(300);
            put_in(putorder, putindex[j]);
            stage == 0 ? set_pin(rsr, 1) : set_pin(rsl, 1);
            while (Check_Err_Num(0, 0) == -3 || Check_Err_Num(0, 2) == -3)
                ;
            stop();
            delay(300);
            if (stage == 0 ? ultrasonic_distance(us_left) : ultrasonic_distance(us_right) < 18)
            {
                stage == 0 ? set_pin(rsr, 1) : set_pin(rsl, 1);
                static double usdisput2 = stage == 0 ? ultrasonic_distance(us_left) : ultrasonic_distance(us_right);
                do
                {
                    usdisput2 = stage == 0 ? ultrasonic_distance(us_left) : ultrasonic_distance(us_right);
                    delay(30);
                } while (usdisput2 < 15.0);
            }
            j++;
        }
        if (flag == 6)
        {
            do
            {
            } while (stage == 0 ? Check_Err_Num(forward, leftward) == -3 : Check_Err_Num(forward, rightward) == -3);
            stop();
            delay(300);
            stage == 0 ? stay_adapt(leftward, leftward) : stay_adapt(rightward, leftward);
            if (stage == 0 ? ultrasonic_distance(us_left) > 5.5 : ultrasonic_distance(us_right) > 5.5)
            {
                stage == 0 ? set_pin(rsl, 1) : set_pin(rsr, 1);
                static double usdisput3 = stage == 0 ? ultrasonic_distance(us_left) : ultrasonic_distance(us_right);
                do
                {
                    usdisput3 = stage == 0 ? ultrasonic_distance(us_left) : ultrasonic_distance(us_right);
                    delay(30);
                } while (usdisput3 > 5.5);
            }
            stop();
            delay(300);
            stage == 0 ? stay_adapt(leftward, leftward) : stay_adapt(rightward, leftward);
            delay(300);
            anti_sm.write(60);
            delay(300);
            stepped_motor(1);
            delay(3200);
            stepped_motor(1);
            delay(3400);
            mys[3].write(0);
            delay(1000);
            break;
        }
        //==============================================
        //ult_pre_status = ult_status;
        
        trailing(forward);
        delay(50);
    }*/
}

void stepped_motor(int dir)
{
    if (dir == 1)
    {
        anti_sm.write(60);
        delay(200);
        digitalWrite(sm_dir, LOW);
        tone(sm_pls, 533, 3000);
        delay(1500);
        tone(sm_pls, 550);
        do
        {
        } while (digitalRead(A9) == 1);
        anti_sm.write(120);
        noTone(sm_pls);
    }
    else
    {
        anti_sm.write(60);
        digitalWrite(sm_dir, HIGH);
        tone(sm_pls, 533, 3000);
        delay(1500);
        tone(sm_pls, 550);
        do
        {
        } while (digitalRead(A9) == 1);
        anti_sm.write(120);
        noTone(sm_pls);
    }
}
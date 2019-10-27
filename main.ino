#include "main_setting.h"

//0:top
//1:middle
//2:down

//========================================================leftstage==========================================================================
//===========================================================================================================================================
//===========================================================================================================================================
#ifdef leftstage
void loop()
{
    uint8_t rfid_read_now[4] = {0, 0, 0, 0};
    //Serial.print("rfid=");
    //Serial.println(rfid_read_now[0]);
    rfid.read(BLOCK_NO, rfid_read_now);
    switch (rfid_read_now[0])
    {
    case 3:
        static bool prece_already3 = false;
        if (!search_rfid(3))
        {
            RFIDRA.data[RFIDRA.top++] = 3;
            delay(300);
        }
        else if (!prece_already3)
        {
            stop();
            delay(300);
            stay_adapt(rightward, backward);
            delay(300);
            rotate_car(0, leftward);
            delay(300);
            stay_adapt(backward, backward);
            delay(300);
            stop();
            delay(300);
            last_status = backward;
            trailing(backward);
            prece_already3 = true;
        }
        break;
    case 4:
        static bool prece_already4 = false;
        if (!search_rfid(4))
        {
            RFIDRA.data[RFIDRA.top++] = 4;
            delay(300);
        }
        else if (!prece_already4)
        {
            stop();
            delay(300);
            stay_adapt(leftward, forward);
            set_pin(rsl, 1);
            static double usdis4 = ultrasonic_distance(us_left);
            do
            {
                usdis4 = ultrasonic_distance(us_left);
                delay(50);
            } while (usdis4 > 18.0);
            stop();
            delay(300);
            stay_adapt(leftward, rightward);
            //=======================
            //=======================
            mys[2].write(180);
            delay(1000);
            //=======================
            //=======================
            set_pin(rsr, 1);
            do
            {
            } while (Check_Err_Num(0, 0) == -3 || Check_Err_Num(0, 2) == -3);
            stop();
            delay(300);
            stay_adapt(backward, forward);
            delay(300);
            //==========================================
            //Serial.print(9);
            //delay(2000);
            //==========================================
            last_status = backward;
            trailing(backward);
            prece_already4 = true;
        }
        break;
    case 5:
        static bool prece_already5 = false;
        if (!search_rfid(5))
        {
            stop();
            delay(300);
            RFIDRA.data[RFIDRA.top++] = 5;
            stay_adapt(rightward, rightward);
            delay(300);
            rotate_car(1, forward);
            delay(300);
            stay_adapt(forward, forward);
            delay(300);
            //============================
            //============================
            anti_sm.write(60);
            delay(300);
            stepped_motor(1);
            delay(3400);
            anti_sm.write(120);
            //============================
            //============================
            last_status = forward;
            trailing(forward);
            delay(200);
        }
        else if (!prece_already5)
        {
            stop();
            delay(300);
            stay_adapt(rightward, rightward);
            delay(300);
            rotate_car(0, backward);
            delay(300);
            stay_adapt(backward, forward);
            delay(300);
            stop();
            delay(200);
            last_status = backward;
            trailing(backward);
            prece_already5 = true;
        }
        break;
    case 7:
        if (!search_rfid(7))
        {
            stop();
            RFIDRA.data[RFIDRA.top++] = 7;
            delay(300);
            stay_adapt(leftward, backward);
            delay(300);
            set_pin(rsl, 1);
            static double usdis7 = ultrasonic_distance(us_left);
            do
            {
                usdis7 = ultrasonic_distance(us_left);
                delay(30);
            } while (usdis7 > 18.0);
            stop();
            delay(300);
            stay_adapt(leftward, leftward);
            //
            mys[0].write(180);
            delay(1000);
            set_pin(rsr, 1);
            do
            {
            } while (Check_Err_Num(0, 0) == -3 || Check_Err_Num(0, 2) == -3);
            stop();
            delay(300);
            //======================================
            //Serial.print(7);
            //delay(2000);
            //======================================
            anti_sm.write(60);
            delay(1000);
            stepped_motor(0);
            delay(3100);
            stepped_motor(0);
            delay(3400);
            anti_sm.write(120);
            //
            set_pin(rsl, 1);
            static double usdis8 = ultrasonic_distance(us_left);
            do
            {
                usdis8 = ultrasonic_distance(us_left);
                delay(30);
            } while (usdis8 > 12.0);
            stop();
            delay(300);
            //stay_adapt(leftward, leftward);

            int16_t rs[4] = {-180, -180, -180, -180};
            set_pin(rs, 1);
            delay(500);
            do
            {
                usdis7 = ultrasonic_distance(us_left);
                //Serial.println(usdis7);
            } while (usdis7 > 40.0); //&& Check_Err_Num(backward, leftward) == -3)

            stop();
            delay(300);
            //=========================
            //=========================
            mys[1].write(180);
            delay(1000);
            //=========================
            //=========================
            int motion_para_down[3] = {190, -255, 0};
            int rs2[4];
            calc_rotating_speed(motion_para_down, rs2);
            set_pin(rs2, 1);
            do
            {
            } while (Check_Err_Num(0, 0) == -3 || Check_Err_Num(0, 2) == -3);
            stop();
            delay(300);
            //=======================================
            //Serial.print(8);
            //delay(2000);
            //=======================================
            stay_adapt(backward, forward);
            anti_sm.write(60);
            delay(300);
            stepped_motor(1);
            delay(3400);
            anti_sm.write(120);
            last_status = backward;
            trailing(backward);
        }
        break;

        /*
#ifdef tesing
    case 5:
        static bool prece_already5 = false;
        if (!search_rfid(5))
        {
            stop();
            delay(300);
            RFIDRA.data[RFIDRA.top++] = 5;
            stay_adapt(rightward, rightward);
            delay(300);
            rotate_car(1, forward);
            delay(300);
            stay_adapt(forward, forward);
            delay(300);
            //============================
            //============================
            anti_sm.write(60);
            delay(300);
            stepped_motor(0);
            delay(3400);
            anti_sm.write(120);
            //============================
            //============================
            set_pin(rsf);
            delay(1000);
            stop();
            delay(300);
            set_pin(rsf, 1);
            delay(800);
            stop();
            delay(300);
            int16_t rsfforce[4] = {255, 255, 255, 255};
            set_pin(rsfforce, 1);
            do
            {
            } while (Check_Err_Num(0, 1) == -3 || Check_Err_Num(0, 3) == -3);
            stop();
            delay(300);
            mys[1].write(180);
            delay(1000);
            set_pin(rsfforce, 1);
            do
            {
            } while (Check_Err_Num(0, 1) == -3 || Check_Err_Num(0, 3) == -3);
            stop();
            delay(300);
            set_pin(rsr, 1);
            static double usdis5 = ultrasonic_distance(us_left);
            do
            {
                usdis5 = ultrasonic_distance(us_left);
                //Serial.println(usdis8);
                delay(50);
            } while (usdis8 < 20.0);
            stop();
            delay(300);
            stay_adapt(leftward, leftward);

            anti_sm.write(60);
            delay(1000);
            stepped_motor(1);
            delay(3100);
            stepped_motor(1);
            delay(3400);
            anti_sm.write(120);

            set_pin(rsl, 1);
            do
            {
                usdis5 = ultrasonic_distance(us_left);
                //Serial.println(usdis8);
                delay(50);
            } while (usdis8 < 16.0);
            stop();
            delay(300);
            stay_adapt(leftward, leftward);
            mys[0].write(180);
            delay(1000);
            set_pin(rsr, 1);
            while (Check_Err_Num(0, 0) == -3 || Check_Err_Num(0, 2) == -3)
                delay(30);
            stop();
            delay(300);
            stay_adapt(backward, forward);
            delay(300);
            last_status = backward;
            trailing(backward);
        }
        else if (!prece_already5)
        {
            stop();
            delay(300);
            stay_adapt(rightward, rightward);
            delay(300);
            rotate_car(0, backward);
            delay(300);
            stay_adapt(backward, forward);
            delay(300);
            stop();
            delay(200);
            last_status = backward;
            trailing(backward);
            prece_already5 = true;
        }
        break;

#endif
*/
    case 8:
        if (!search_rfid(8))
        {
            stop();
            delay(300);
            RFIDRA.data[RFIDRA.top++] = 8;
            stay_adapt(rightward, backward);
            delay(300);
            set_pin(rsr, 1);
            static double usdis8 = ultrasonic_distance(us_right);
            do
            {
                usdis8 = ultrasonic_distance(us_right);
                //Serial.println(usdis8);
                delay(50);
            } while (usdis8 > 10.0);
            stop();
            delay(300);
            stay_adapt(rightward, leftward);
            //=========================
            //=========================
            mys[3].write(180);
            delay(1000);
            //=========================
            //=========================
            set_pin(rsl, 1);
            do
            {
                //delay(30);
            } while (Check_Err_Num(0, 0) == -3 || Check_Err_Num(0, 2) == -3);
            stop();
            delay(300);

            //=================================================
            /*for (int i = 10; i <= 12; i++)
            {
                Serial.print(i);
                while (1)
                {
                    if (Serial.available() > 0)
                    {
                        cubecolor[i - 10] = Serial.read() - 48;
                        while (Serial.available() > 0)
                            Serial.read();
                        break;
                    }
                }
                delay(1000);
            }*/
            //=================================================

            stay_adapt(backward, forward);
            delay(300);
            last_status = backward;
            trailing(backward);
        }
        break;
    case 9:
        if (!search_rfid(9))
        {
            stop();
            delay(300);
            RFIDRA.data[RFIDRA.top++] = 9;
            stay_adapt(leftward, backward);
            delay(300);
            rotate_car(0, rightward);
            delay(300);
            stay_adapt(forward, forward);
            delay(300);
            last_status = forward;
            trailing(forward);
        }
        put(cubecolor, boxcolor, 0);
        break;
    default:
        trailing(last_status);
        break;
    }
    delay(30);
}
#endif

//========================================================rightstage=========================================================================
//===========================================================================================================================================
//===========================================================================================================================================
#ifdef rightstage
void loop()
{
    uint8_t rfid_read_now[4] = {0, 0, 0, 0};
    //Serial.print("rfid=");
    //Serial.println(rfid_read_now[0]);
    rfid.read(BLOCK_NO, rfid_read_now);
    switch (rfid_read_now[0])
    {
    case 3:
        static bool prece_already3 = false;
        if (!search_rfid(3))
        {
            RFIDRA.data[RFIDRA.top++] = 3;
            delay(300);
        }
        else if (!prece_already3)
        {
            stop();
            delay(300);
            stay_adapt(leftward, backward);
            delay(300);
            rotate_car(1, rightward);
            delay(300);
            stay_adapt(backward, backward);
            delay(300);
            stop();
            delay(300);
            last_status = backward;
            trailing(backward);
            prece_already3 = true;
        }
        break;
    case 4:
        static bool prece_already4 = false;
        if (!search_rfid(4))
        {
            RFIDRA.data[RFIDRA.top++] = 4;
            delay(300);
        }
        else if (!prece_already4)
        {
            stop();
            delay(300);
            stay_adapt(rightward, forward);
            set_pin(rsr, 0);
            static double usdis4 = ultrasonic_distance(us_right);
            do
            {
                usdis4 = ultrasonic_distance(us_right);
                delay(50);
            } while (usdis4 > 18.0);
            stop();
            delay(300);
            stay_adapt(rightward, leftward);
            //=======================
            //=======================
            mys[2].write(180);
            delay(1000);
            //=======================
            //=======================
            set_pin(rsl, 0);
            do
            {
            } while (Check_Err_Num(0, 0) == -3 || Check_Err_Num(0, 2) == -3);
            stop();
            delay(300);
            stay_adapt(backward, forward);
            delay(300);
            //==========================================
            //Serial.print(9);
            //delay(2000);
            //==========================================
            last_status = backward;
            trailing(backward);
            prece_already4 = true;
        }
        break;
    case 5:
        static bool prece_already5 = false;
        if (!search_rfid(5))
        {
            stop();
            delay(300);
            RFIDRA.data[RFIDRA.top++] = 5;
            stay_adapt(leftward, leftward);
            delay(300);
            rotate_car(0, forward);
            delay(300);
            stay_adapt(forward, forward);
            delay(300);
            //============================
            //============================
            anti_sm.write(60);
            delay(300);
            stepped_motor(1);
            delay(3400);
            anti_sm.write(120);
            //============================
            //============================
            last_status = forward;
            trailing(forward);
            delay(300);
        }
        else if (!prece_already5)
        {
            stop();
            delay(300);
            stay_adapt(leftward, leftward);
            delay(300);
            rotate_car(1, backward);
            delay(300);
            stay_adapt(backward, forward);
            delay(300);
            stop();
            delay(200);
            last_status = backward;
            trailing(backward);
            prece_already5 = true;
        }
        break;
    case 7:
        if (!search_rfid(7))
        {
            stop();
            RFIDRA.data[RFIDRA.top++] = 7;
            delay(300);
            stay_adapt(rightward, backward);
            delay(300);
            set_pin(rsr, 0);
            static double usdis7 = ultrasonic_distance(us_right);
            do
            {
                usdis7 = ultrasonic_distance(us_right);
                delay(30);
            } while (usdis7 > 18.0);
            stop();
            delay(300);
            //
            mys[0].write(180);
            delay(1000);
            set_pin(rsl, 1);
            do
            {
            } while (Check_Err_Num(0, 0) == -3 || Check_Err_Num(0, 2) == -3);
            stop();
            delay(300);
            //======================================
            //Serial.print(7);
            //delay(2000);
            //======================================
            anti_sm.write(60);
            delay(1000);
            stepped_motor(0);
            delay(3100);
            stepped_motor(0);
            delay(3400);
            anti_sm.write(120);
            //
            set_pin(rsr, 1);
            static double usdis8 = ultrasonic_distance(us_right);
            do
            {
                usdis8 = ultrasonic_distance(us_right);
                delay(30);
            } while (usdis8 > 13.0);
            stop();
            delay(300);
            //stay_adapt(leftward, leftward);

            int16_t rs[4] = {-180, -180, -180, -180};
            set_pin(rs, 0);
            delay(500);
            do
            {
                usdis7 = ultrasonic_distance(us_right);
                //Serial.println(usdis7);
            } while (usdis7 > 40.0); //&& Check_Err_Num(backward, rightward) == -3)
            stop();
            delay(300);
            //=========================
            //=========================
            mys[1].write(180);
            delay(1000);
            //=========================
            //=========================
            int motion_para_down[3] = {-190, -255, 0};
            int rs2[4];
            calc_rotating_speed(motion_para_down, rs2);
            set_pin(rs2, 0);
            delay(700);
            do
            {
            } while (Check_Err_Num(0, 0) == -3 || Check_Err_Num(0, 2) == -3);
            stop();
            delay(300);
            //=======================================
            // Serial.print(8);
            //delay(2000);
            //=======================================
            stay_adapt(backward, forward);
            anti_sm.write(60);
            delay(300);
            stepped_motor(1);
            delay(3400);
            anti_sm.write(120);
            last_status = backward;
            trailing(backward);
        }
        break;
    case 8:
        if (!search_rfid(8))
        {
            stop();
            delay(300);
            RFIDRA.data[RFIDRA.top++] = 8;
            stay_adapt(leftward, backward);
            delay(300);
            set_pin(rsl, 0);
            static double usdis8 = ultrasonic_distance(us_left);
            do
            {
                usdis8 = ultrasonic_distance(us_left);
                //Serial.println(usdis8);
                delay(50);
            } while (usdis8 > 10.0);
            stop();
            delay(300);
            stay_adapt(leftward, rightward);
            //=========================
            //=========================
            mys[3].write(180);
            delay(1000);
            //=========================
            //=========================
            set_pin(rsr, 0);
            do
            {
                //delay(30);
            } while (Check_Err_Num(0, 0) == -3 || Check_Err_Num(0, 2) == -3);
            stop();
            delay(300);

            //=================================================
            /*for (int i = 10; i <= 12; i++)
            {
                Serial.print(i);
                while (1)
                {
                    if (Serial.available() > 0)
                    {
                        cubecolor[i - 10] = Serial.read() - 48;
                        while (Serial.available() > 0)
                            Serial.read();
                        break;
                    }
                }
                delay(1000);
            }*/
            //=================================================

            stay_adapt(backward, forward);
            delay(300);
            last_status = backward;
            trailing(backward);
        }
        break;
    case 9:
        if (!search_rfid(9))
        {
            stop();
            delay(300);
            RFIDRA.data[RFIDRA.top++] = 9;
            stay_adapt(rightward, backward);
            delay(300);
            rotate_car(1, leftward);
            delay(300);
            stay_adapt(forward, forward);
            delay(300);
            last_status = forward;
            trailing(forward);
        }
        put(cubecolor, boxcolor, 1);
        break;
    default:
        trailing(last_status);
        break;
    }
    delay(30);
}
#endif

//===================================================necessary funcion in this program=======================================================
//===========================================================================================================================================
//===========================================================================================================================================
bool search_rfid(uint8_t no)
{
    for (int i = 0; i < 20; i++)
        if (RFIDRA.data[i] == no)
            return true;
        else
        {
            continue;
        }

    return false;
}

//====================================other blocked part for tesing using the pre-compile instructions========================================
//============================================================================================================================================
//============================================================================================================================================
#ifdef testcode
void loop()
{
    /*pinMode(A13, OUTPUT);
    pinMode(A7, INPUT);
    digitalWrite(A13, HIGH);
    if (digitalRead(A7) == 1)
        sm_adapt();
    delay(5000);*/
    /*Serial.print("modelno=");
    Serial.println(0);
    for_stay_adapt(0);
    delay(500);*/
    //rotate_car(0,0);
    //delay(1000);
    //stepped_motor(1);
    //set_pin(rsf);
    //delay(1000);
    //stop();
    //delay(5000);
    //trailing(forward);
    //set_pin(rsl);
    //delay(1000);
    //stop();
    //delay(1000);
    /* Servo wesd;
    wesd.attach(8);
    wesd.write(0);
    delay(2000);
    wesd.write(90);
    delay(2000);
    wesd.write(0);
    delay(2000);
    wesd.write(90);
    delay(2000);
    wesd.write(0);
    delay(2000);
    wesd.write(90);
    delay(2000);
    wesd.write(0);
    delay(2000);
    wesd.write(90);
    delay(2000);*/
    /*while (1)
    {
        anti_sm.write(120);
        delay(300);
        anti_sm.write(60);
        delay(300);
        stepped_motor(0);
        /*while (1)
            if (digitalRead(A9) != 1)
            {
                anti_sm.write(120);
                break;
            }
        delay(300);
    }
    /*pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
    delay(3000);

    for (int i = 0; i < 6; i++)
    {
        Serial.print(boxcolor[i]);
        delay(500);
    }
    delay(1000000);*/
    /*int stage = 1;
    //stage == 0 ? set_pin(rsl, 1) : set_pin(rsr, 1);
    static double usdisput3 = stage == 0 ? ultrasonic_distance(us_left) : ultrasonic_distance(us_right);
    do
    {
        usdisput3 = stage == 0 ? ultrasonic_distance(us_left) : ultrasonic_distance(us_right);
        Serial.println(usdisput3);
        delay(500);
    } while (usdisput3 > 5.5);*/
    delay(1000);
    Serial.print(1);
    int i = 0;
    while (1)
    {
        if (Serial.available() > 0)
        {
            delay(1000);
            while (i < 9)
            {
                if (i < 6)
                {
                    boxcolor[5 - i] = Serial.read() - 48;
                    i++;
                }
                else
                {
                    cubecolor[i - 6] = Serial.read() - 48;
                    i++;
                }
            }
            break;
        }
    }
    delay(3000);
    for (int i = 0; i < 9; i++)
    {
        if (i < 6)
        {
            Serial.print(boxcolor[i]);
        }
        else
        {
            Serial.print(cubecolor[i - 6]);
        }
    }
    delay(1000000);
}
#endif

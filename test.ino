#include "move.h"
#include "trailing.h"
#include "Rfid_D_Think_M50.h"

#define BLOCK_NO 0
Rfid_D_Think_M50 rfid(Serial1);
uint8_t data[4] = {0, 0, 0, 0};

void setup()
{
    //红外设置
    pinMode(f0, INPUT);
    pinMode(f1, INPUT);
    pinMode(f2, INPUT);
    pinMode(b0, INPUT);
    pinMode(b1, INPUT);
    pinMode(b2, INPUT);

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
    digitalWrite(48, HIGH);
    Serial.begin(9600);
    rfid.enableAntenna(rfid.ISO15693);
}

void loop()
{
    trailing_straight();
    if (rfid.read(BLOCK_NO, data))
        if (data[0] == 1)
        {
            stop();
            //digitalWrite(48,LOW);
            for (;;){}
        }
    delay(30);
}

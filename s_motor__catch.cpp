#include "s_moto__catch.h"

void sm_adapt()
{
    int x = millis();
    stepped_motor(1);
    while (millis() < x + 2000)
        if (digitalRead(A7) == 0)
        {
            noTone(sm_pls);
            return;
        }
    noTone(sm_pls);
    delay(100);
    stepped_motor(0);
    x = millis();
    while (millis() < x + 2000)
        if (digitalRead(A7) == 0)
        {
            noTone(sm_pls);
            return;
        }
    stepped_motor(0);
    x = millis();
    while (millis() < x + 2000)
        if (digitalRead(A7) == 0)
        {
            noTone(sm_pls);
            return;
        }
}
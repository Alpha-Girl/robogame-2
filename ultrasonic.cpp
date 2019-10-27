#include "ultrasonic.h"

float ultrasonic_distance(int num)
{

    if (num == us_left)
    {
        digitalWrite(trigl, LOW);
        delayMicroseconds(5);
        digitalWrite(trigl, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigl, LOW);
        long duration = pulseIn(echol, HIGH);
        return (((float)duration / 2.0) / 29.1);
    }
    if (num == us_right)
    {
        digitalWrite(trigr, LOW);
        delayMicroseconds(5);
        digitalWrite(trigr, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigr, LOW);
        long duration = pulseIn(echor, HIGH);
        return (((float)duration / 2.0) / 29.1);
    }
}

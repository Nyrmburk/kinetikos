/*
 * File:   Math.cpp
 * Author: nyrmburk
 *
 * Created on May 6, 2018, 9:31 AM
 */

#include <math.h>
#include "Math.h"

float clamp(float value, float min, float max) {

    if (value < min) {
        value = min;
    } else if (value > max) {
        value = max;
    }
    return value;
}

float wrap(float value, float min, float max) {

    return fmod(min + value, max - min) - min;
}

float angleDifference(float angleA, float angleB) {

    return wrap(fmod((angleA - angleB + M_PI_2),  M_PI) - M_PI_2, -M_PI_2, M_PI_2);
}

float solve_sas(float sideA, float sideB, float angle) {

    float sideC = sideA * sideA + sideB * sideB;
    sideC -= 2 * sideA * sideB * cos(angle);
    sideC = sqrt(sideC);
    return sideC;
}

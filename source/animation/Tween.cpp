/* 
 * File:   tween.cpp
 * Author: nyrmburk
 * 
 * Created on June 24, 2017, 10:26 AM
 */

#include "Tween.h"

float easeNone(float k) {
    return 0;
}

float easeLinear(float k) {
    return k;
}

float easeQuadraticIn(float k) {
    return k * k;
}

float easeQuadraticOut(float k) {
    return - k * (k - 2);
}

float easeQuadraticInOut(float k) {
    if ((k *= 2) < 1) 
        return 0.5 * k * k;
    return -0.5 * (--k * (k - 2) - 1);
}


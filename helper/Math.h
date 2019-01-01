/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Math.h
 * Author: nyrmburk
 *
 * Created on May 6, 2018, 9:31 AM
 */

#ifndef MATH_H
#define MATH_H

#define M_LN10 2.30258509299404568402
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define M_1_SQRTPI 0.564189583547756286948
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2 1.41421356237309504880
#define M_SQRT_2 0.707106781186547524401

    float clamp(float value, float min, float max);
    float wrap(float value, float min, float max);
    float angleDifference(float angleA, float angleB);
    float solve_sas(float sideA, float sideB, float angle);
    float mapRange(float x, float fromA, float fromB, float toA, float toB);

#endif /* MATH_H */


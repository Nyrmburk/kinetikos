/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   leg.h
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 2:24 PM
 */

#ifndef LEG_H
#define LEG_H

#include "Bone.h"
#include "../matrix/vec3.h"

#define BONES_PER_LEG 3

const Vec3 COXA_AXIS = {0, 0, 1}; // z is up
const Vec3 FEMUR_AXIS = {0, -1, 0}; // y is forward
const Vec3 TIBIA_AXIS = FEMUR_AXIS;

class Leg {
public:
    Leg();

    Vec3 offset; // could be mat4
    Bone coxa;
    Bone femur;
    Bone tibia;
    
    void solveForward(const Mat4 *orientation, float joints[], const Vec3 *foot);
    int solveInverse(const Mat4 *orientation, const Vec3 *foot, float joints[], const Vec3 *forward);
};

#endif /* LEG_H */


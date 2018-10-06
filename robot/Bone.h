/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bone.h
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 2:24 PM
 */

#ifndef BONE_H
#define BONE_H

#include "../matrix/mat4.h"
#include "../matrix/vec3.h"

class Bone {
public:
    Bone();
    
    float length;
    float angle;
    float range;
    
    void solve(const Vec3 *axis, const float joint, Mat4 *matrix);
};

#endif /* BONE_H */


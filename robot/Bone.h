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

#include "../helper/Serializable.h"
#include "../matrix/mat4.h"
#include "../matrix/vec3.h"

class Bone : public Serializable {
public:
    Bone();
    
    float length;
    float angle;
    float range;
    
    void solve(const Vec3 *axis, const float joint, Mat4 *matrix);

    void serialize(DataView* data) {
        data->writeFloat(length);
        data->writeFloat(angle);
        data->writeFloat(range);
    }

    void deserialize(DataView* data) {
        length = data->readFloat();
        angle = data->readFloat();
        range = data->readFloat();
    }
};

#endif /* BONE_H */


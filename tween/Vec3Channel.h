/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Vec3Channel.h
 * Author: nyrmburk
 *
 * Created on April 28, 2018, 3:13 PM
 */

#ifndef VEC3_CHANNEL_H
#define VEC3_CHANNEL_H

#include "TweenChannel.h"
#include "../matrix/vec3.h"

class Vec3Channel : public TweenChannel<Vec3> {
public:
    Vec3Channel(Vec3& vec) : TweenChannel(vec) {}
    
    void ease(Vec3* start, Vec3* end, float k, Vec3* result) {
        subtractv3(end, start, result);
        multiplyv3s(result, k, result);
        addv3(start, result, result);
    }
};

#endif /* VEC3_CHANNEL_H */


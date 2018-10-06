/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GaitController.h
 * Author: nyrmburk
 *
 * Created on May 8, 2018, 10:00 PM
 */

#ifndef GAITCONTROLLER_H
#define GAITCONTROLLER_H

#include "AnimationController.h"
#include "../matrix/mat4.h"

class GaitController {
public:
    GaitController(AnimationController animation);
    virtual ~GaitController();
    
    void step(float delta, Mat4 &futureOrientation);
private:
    AnimationController animation;
};

#endif /* GAITCONTROLLER_H */


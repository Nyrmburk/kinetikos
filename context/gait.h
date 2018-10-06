/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   gait_context.h
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 11:43 PM
 */

#ifndef GAIT_CONTEXT_H
#define GAIT_CONTEXT_H

#include "../matrix/vec2.h"

class gait {
public:
    gait();
    virtual ~gait();
private:
    Vec2* translation;
    float rotation;
};

#endif /* GAIT_CONTEXT_H */


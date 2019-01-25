/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MovementControl.h
 * Author: nyrmburk
 *
 * Created on May 6, 2018, 10:18 PM
 */

#ifndef MOVEMENTCONTROL_H
#define MOVEMENTCONTROL_H

#include "../matrix/vec3.h"

#include <math.h>

#include "NavigationControl.h"

class Robot;

class MovementControl {
public:
    MovementControl(Robot *robot);
    virtual ~MovementControl();
    
    typedef struct {
        float angle;
        Vec3 position;
        Vec3 velocity;
    } Orientation;
    
    typedef struct {
        float maxAcceleration = 500;
        float maxSpeed = 500;
        float maxAngularAcceleration = M_PI;
        float maxAngularSpeed = M_PI;
    } Movement;
    Movement movement;
    
    Orientation* currentOrientation();
    void getOrientation(float future, Orientation *result);
    
    void step(float delta);
    void planVelocity(NavigationControl navigation);
private:

    Robot *robot;
    Orientation orientation;
};

#endif /* MOVEMENTCONTROL_H */


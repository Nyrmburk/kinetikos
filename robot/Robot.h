/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Robot.h
 * Author: nyrmburk
 *
 * Created on May 4, 2018, 9:07 PM
 */

#ifndef ROBOT_H
#define ROBOT_H

#include "Body.h"
#include "../mapping/World.h"
#include "../control/MotorControl.h"
#include "../control/FootControl.h"

class MotorControl;
class FootControl;

class Robot {
public:
    Robot();
    virtual ~Robot();
    
    Body* getBody();
    World* getWorld();
    
    void simulationStep(float delta);
    void animationStep(float delta);
private:
    Body *body;
    World *world;
    
    FootControl *footControl;
    MotorControl *motorControl;
};

#endif /* ROBOT_H */


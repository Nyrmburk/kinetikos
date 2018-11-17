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
#include "../control/Foot.h"
#include "../control/Joints.h"
#include "../control/MotorControl.h"

class Foot;
class FootControl;
class Joints;
class JointsControl;
class MotorControl;

class Robot {
public:
    Robot();
    virtual ~Robot();
    
    Body* getBody();
    World* getWorld();
    
    void simulationStep(float delta);
    void animationStep(float delta);
private:
    Body* body;
    World* world;
    
    Joints* joints;
    Foot* feet;
    Bezier3Channel* footPaths;

    MotorControl* motorControl;
    JointsControl* jointsControl;
    FootControl* footControl;
};

#endif /* ROBOT_H */

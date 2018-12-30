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
#include "../mapping/MotionPlan.h"
#include "../animation/AnimationClip.h"
#include "../control/Foot.h"
#include "../control/Joints.h"
#include "../control/MotorControl.h"

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

    void setAnimation(AnimationClip* clip) {
        currentAnimation = clip;
    }

    MotionPlan* getMotionPlan() {
        return plan;
    }

    void setMotionPlan(MotionPlan* plan) {
        this->plan = plan;
    }

    Vec3* getFeetHome() {
        return feetHome;
    }

    void setFeetHome(Vec3 feet[]) {
        feetHome = feet;
    }

    Mat4* getOrientation() {
        return &orientation;
    }

    Mat4* getBodyOrientation() {
        return &bodyOrientation;
    }

    Joints* getJoints() {
        return joints;
    }

    Vec3* getFeet() {
        return feet;
    }

    Bezier3Channel* getFootPaths() {
        return footPaths;
    }
    
    void simulationStep(float delta);
    void animationStep(float delta);
private:
    Body* body;
    World* world;
    AnimationClip* currentAnimation = nullptr;
    MotionPlan* plan = nullptr;

    Mat4 bodyOrientation;
    Joints* joints;
    Vec3* feet;
    Vec3* feetHome;
    Bezier3Channel* footPaths;
    Mat4 orientation;

    MotorControl* motorControl;
    JointsControl* jointsControl;
    FootControl* footControl;
};

#endif /* ROBOT_H */

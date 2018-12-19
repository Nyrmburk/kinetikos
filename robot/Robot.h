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

#include <deque>

#include "Body.h"
#include "Gait.h"
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

    void setMotionPlan(MotionPlan* plan) {
        this->plan = plan;
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
    
    void simulationStep(float now, float delta);
    void animationStep(float now, float delta);
private:
    Body* body;
    World* world;
    AnimationClip* currentAnimation = nullptr;
    MotionPlan* plan = nullptr;
    Gait* gait;

    Mat4 bodyOrientation;
    Joints* joints;
    Vec3* feet;
    Vec3* feetHome;
    Bezier3Channel* footPaths;
    Mat4 orientation;

    MotorControl* motorControl;
    JointsControl* jointsControl;
    FootControl* footControl;

    float gaitCursor = 0;
    float simTime = 0;
    
    struct StepFrame {
        float simTime = 0;
        float landTime = 0;
        float liftTime = 0;

        bool wasGrounded = false;
        Mat4 accumulatedGroundedOrientation = {};
        int groundedIterations = 0;
    };
    deque<StepFrame>* steps;
    StepFrame** workingSteps;

    bool simGait(float time, Mat4* orientation, StepFrame& frame, Gait& gait, float cursor);
};

#endif /* ROBOT_H */

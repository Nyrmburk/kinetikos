/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   foot_control.h
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 2:21 PM
 */

#ifndef FOOT_CONTROL_H
#define FOOT_CONTROL_H

#include "../robot/Robot.h"

#include "../matrix/vec2.h"
#include "../matrix/vec3.h"
#include "../matrix/mat4.h"

class Robot;

class FootControl {
public:
    FootControl(int feet);
    virtual ~FootControl();
    
    typedef struct {
        unsigned int iterations = 10;
        float jointNeutralityHeuristic = 1;
        float physicsHeuristic = 1;
        float avoidFootHeuristic = 1;
        float avoidFootMinRadius = 10; // mm radius
        float worldAngleHeuristic = 1;
        float maxWorldAngle = 0.523599; // 30 degrees
        float stepSize = 1;
        float derivativeStepSize = 0.5;
    } FootPlanningHeuristics;
    FootPlanningHeuristics heuristics;
    
    typedef struct {
        float radius = 5;
        float maxRadius = 80;
        float ringIterations = 5;
    } TargetSearchVariables;
    TargetSearchVariables search;
    
    void step(Robot *robot, float delta);
    const Vec3* getTargets();
    
    float calculateCost(Robot *robot, Vec3 *target, int leg);
    
    float findNearbyValidTarget(Robot *robot, Vec3 *guessTarget, int foot, Vec3 *returnTarget);

private:
    // I can't do Body because it doesn't have info about the velocity
    // This is what I used RobotContext for.
    // I want to have a generic call to get the data from somewhere, 
    //      but I have to stay synchronized.
    // Where even IS velocity supposed to be?
    // also I need the world
    // should I worry about fetching the data myself from a context or have it passed in?
    // passed in would make the most sense from a design standpoint
    //      that way everything is just glue code right?
    // it means no unintended behavior
    // nah it should get the data itself so that it's easier to use
    // ahhh, but what about the fucking possibility of motion planning.
    // then there's the fact that nothing is the same and the context is different every time
    // well, it all just goes off of a base context
    // so each fork would just go normally.
    // mayyybe I should start adding in dupe methods?
    // nah. I'll do that later
    void planTarget(Robot *robot, int foot);
    Vec3 *feetTargets;
};

#endif /* FOOT_CONTROL_H */


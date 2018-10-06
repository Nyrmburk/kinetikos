
/* 
 * File:   foot_control.cpp
 * Author: nyrmburk
 * 
 * Created on June 23, 2017, 2:21 PM
 */

#include <cstdlib>
#include <math.h>
#include "helper/Math.h"
#include "FootControl.h"

FootControl::FootControl(int feet) {
    feetTargets = new Vec3[feet];
}

FootControl::~FootControl() {
    delete feetTargets;
}

void FootControl::step(Robot *robot, float delta) {
    
}

const Vec3* FootControl::getTargets() {
    // TODO: lock
    return feetTargets;
}

/**
 * use following heuristics to calculate cost
 *      joint angle neutrality
 *          i.e. angles near the edge of actuation are more expensive
 *      actual physics cost of supporting weight for joint angles
 *      avoid other feet to avoid tripping
 *      flat spot on surface (nothing steep or missing)
 */
float FootControl::calculateCost(Robot *robot, Vec3 *target, int leg) {
    // perform ik solve for target
    
    // angle neutrality
    // if |angle| > |range| return -1
    
    // angular force of supporting weight for angles
    // I think it's leg length * sin(angle)
    //      where angle is the angle between the leg and gravity
    //      YAY vectors!
    
    // spring force of distance to surrounding feet
    
    // how flat the world is in the target location
    // basically the angle which would be the derivative of the world
}

float randFloat() {
    return ((float) std::rand()) / RAND_MAX;
}

/**
 * right now this tests a bunch of random points in a certain ring and then 
 * increases the size of that ring.
 * in the future, I think a rewrite to a spiral would work too and maybe faster
 */
float FootControl::findNearbyValidTarget(Robot *robot, Vec3 *guessTarget, int foot, Vec3 *returnTarget) {
    float cost = calculateCost(robot, guessTarget, foot);
    
    while (search.radius < search.maxRadius && cost < 0) {
        for (int i = 0; i < search.ringIterations; i++ ) {
            float angle = randFloat() * (M_PI + M_PI);
            float radiusJiggle = search.radius + randFloat() * search.radius;
            returnTarget->x = guessTarget->x + cos(angle) * radiusJiggle;
            returnTarget->y = guessTarget->y + sin(angle) * radiusJiggle;
            cost = calculateCost(robot, returnTarget, foot);
            if (cost >= 0)
                break;
        }
        search.radius += search.radius;
        search.ringIterations += search.ringIterations;
    }
    return cost;
}

/** 
 * steps:
 * get time of foot down
 * get robot transformation matrix for future position
 * use cost function in gradient descent to find optimal target
 */
void FootControl::planTarget(Robot *robot, int foot) {
    
    // get world
    
    // get sequencer foot down target time
    float time = 0;
    
    // get planned matrix at time
    Mat4 matrix;
    
    // get planned velocity at time
    Vec3 direction;
    v2asv3(nullptr, 0, &direction);
    float speed = normalizedv3(&direction, &direction);
    
    Vec3 target;
    // get coxa location
    Vec3 coxaOffset;
    // get coxa length
    float coxaLength = 1;
    // get offset from direction * (speed + coxaLength)
    multiplyv3s(&direction, speed + coxaLength, &target);
    // add coxa location to target
    addv3(&coxaOffset, &target, & target);
    // project down to world
    
    // gradient descent
    Vec3 testTarget;
    float baseCost = findNearbyValidTarget(robot, &target, foot, &testTarget);
    Vec3 derivative;
    float stepSize = 1;
    for (int i = 0; i < heuristics.iterations; i++) {
        
        // numerical differentiation
        // sometimes test different testTarget because cost is not continuous
        // get x derivative
        testTarget.x += stepSize;
        derivative.x = calculateCost(robot, &testTarget, foot);
        if (derivative.x < 0) { // testing cost from opposite side of source
            testTarget.x = target.x - stepSize;
            derivative.x = -calculateCost(robot, &testTarget, foot);
        }
        derivative.x = (derivative.x - baseCost) / stepSize;
        testTarget.x = target.x;
        // get y derivative
        testTarget.y += stepSize;
        derivative.y = calculateCost(robot, &testTarget, foot);
        if (derivative.y < 0) { // testing cost from opposite side of source
            testTarget.y = target.y - stepSize;
            derivative.y = -calculateCost(robot, &testTarget, foot);
        }
        derivative.y = (derivative.y - baseCost) / stepSize;
        testTarget.y = target.y;
        
        // testTarget -= derivative * stepSize
        multiplyv3s(&derivative, stepSize, &derivative);
        subtractv3(&testTarget, &derivative, &testTarget);
        
        setv3(&testTarget, &target);
        
        baseCost = calculateCost(robot, &target, foot);
    }
    
    setv3(&target, &feetTargets[foot]);
}


/* 
 * File:   Robot.cpp
 * Author: nyrmburk
 * 
 * Created on May 4, 2018, 9:07 PM
 */

#include "Robot.h"
#include "Config.h"
#include <algorithm>

Robot::Robot() {
    Config cfg("config/robot.json");

    body = new Body();
    cfg.getBody(body);

    int legsCount = body->legsCount;

    joints = new Joints[legsCount];
    feet = new Vec3[legsCount];

    identitym4(&orientation);
}

Robot::~Robot() {
    delete[] joints;
    delete[] feet;
}

Body* Robot::getBody() {
    return body;
}

World* Robot::getWorld() {
    return world;
}

void Robot::simulationStep(float now, float delta) {
}

void Robot::animationStep(float now, float delta) {
    if (this->currentAnimation) {
        this->currentAnimation->step(delta);
    }

    for (int i = 0; i < body->legsCount; i++) {
        body->legs[i].solveInverse(&bodyOrientation, &feet[i], &joints[i], nullptr);
    }

    plan->step(delta);
    plan->orientationAt(0, &orientation);
    //cout << orientation.m[12] << ", " << orientation.m[13] << endl;
}

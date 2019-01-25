/* 
 * File:   Robot.cpp
 * Author: nyrmburk
 * 
 * Created on May 4, 2018, 9:07 PM
 */

#include "Robot.h"

#include <algorithm>

Robot::Robot(Config& config) {
    body = new Body();
    config.getBody(body);

    int legsCount = body->legsCount;

    joints = new Joints[legsCount];
    feet = new Vec3[legsCount];

    identitym4(&orientation);

    workspaces = new Workspace[legsCount];
    for (int i = 0; i < legsCount; i++) {
        workspaces[i].generate(body->legs[i]);
    }
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

void Robot::simulationStep(float delta) {
}

void Robot::animationStep(float delta) {
    if (this->currentAnimation) {
        this->currentAnimation->step(delta);
    }

    for (int i = 0; i < body->legsCount; i++) {
        body->legs[i].solveInverse(&bodyOrientation, &feet[i], &joints[i], nullptr);
    }

    plan->step(delta);
    plan->orientationAt(0, &orientation);
}

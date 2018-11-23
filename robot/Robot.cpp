/* 
 * File:   Robot.cpp
 * Author: nyrmburk
 * 
 * Created on May 4, 2018, 9:07 PM
 */

#include "Robot.h"
#include "Config.h"

Robot::Robot() {
    Config cfg("config/robot.json");

    body = new Body();
    cfg.getBody(body);

    int legsCount = body->legsCount;

    joints = new Joints[legsCount];
    feet = new Vec3[legsCount];
}

Robot::~Robot() {
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

}

/* 
 * File:   Robot.cpp
 * Author: nyrmburk
 * 
 * Created on May 4, 2018, 9:07 PM
 */

#include "Robot.h"

Robot::Robot() {
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

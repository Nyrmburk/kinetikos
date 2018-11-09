#include "Joints.h"
#include "../robot/Robot.h"

void JointsControl::control(Robot* robot, const Foot* foot, Joints* returnJoints) {
    Body body = *robot->getBody();
    body.legs->solveInverse(&body.orientation, &foot->foot, returnJoints->joints, nullptr);
}


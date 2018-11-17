#include "Joints.h"
#include "../robot/Robot.h"

void JointsControl::control(Robot* robot, const Vec3* foot, Joints* returnJoints) {
    Body body = *robot->getBody();
    body.legs->solveInverse(&body.orientation, foot, returnJoints->joints, nullptr);
}


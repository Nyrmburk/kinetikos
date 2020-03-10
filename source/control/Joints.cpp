#include "Joints.h"
#include "../robot/Robot.h"

void JointsControl::control(Robot* robot, Leg* leg, const Vec3* foot, Joints* returnJoints) {
    leg->solveInverse(robot->getOrientation(), foot, returnJoints, nullptr);
}


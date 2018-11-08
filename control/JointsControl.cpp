#include "JointsControl.h"

void JointsControl::control(Robot* robot, Joints joints[], const Foot feet[]) {
    Body body = *robot->getBody();
    for (size_t i = 0; i < robot->getBody()->legsCount; i++) {
        body.legs[i].solveInverse(&body.orientation, &feet[i].foot, joints[i].joints, nullptr);
    }
}


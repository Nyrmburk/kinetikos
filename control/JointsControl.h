#ifndef JOINTS_CONTROL_H
#define JOINTS_CONTROL_H

#include "../robot/Robot.h" // circular reference
#include "Joints.h"
#include "Foot.h"

class Robot;

class JointsControl {
    // compute the values for joints from the value of feet
    void control(Robot* robot, Joints joints[], const Foot feet[]);
};

#endif /* JOINTS_CONTROL_H */

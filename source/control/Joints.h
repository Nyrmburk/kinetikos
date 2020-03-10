#ifndef JOINTS_H
#define JOINTS_H

#include "../matrix/vec3.h"
#include "../helper/Serializable.h"
#include "../robot/Leg.h"

class Robot;

class JointsControl {
public:
    // compute the values for joints from the foot
    void control(Robot* robot, Leg* leg, const Vec3* foot, Joints* returnJoints);
};

#endif /* JOINTS_H */

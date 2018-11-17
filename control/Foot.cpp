#include "Foot.h"
#include "../robot/Robot.h"

void FootControl::control(Robot* robot, const Bezier3Channel* path, float time, Vec3* returnFoot) {
    path->step(time, *returnFoot);
}

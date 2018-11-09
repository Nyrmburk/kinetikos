#include "Foot.h"
#include "../robot/Robot.h"

void FootControl::control(Robot* robot, const Vec3Channel* path, float time, Foot* returnFoot) {
    path->step(time, returnFoot->foot);
}

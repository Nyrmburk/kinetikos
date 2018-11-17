#ifndef FOOTPATH_H
#define FOOTPATH_H

#include "../matrix/vec3.h"
#include "../tween/Bezier3Channel.h"
#include "Foot.h"

class FootPathControl {
    void control(Robot* robot, const Vec3* target, Bezier3Channel* returnPath);
};

#endif /* FOOTPATH_H */

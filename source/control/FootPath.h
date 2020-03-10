#ifndef FOOTPATH_H
#define FOOTPATH_H

#include "../matrix/vec3.h"
#include "../animation/Bezier3Channel.h"
#include "Foot.h"

class FootPathControl {
public:
    void control(Robot* robot,
            float sourceTime, const Vec3* source,
            float targetTime, const Vec3* target,
            Bezier3Channel* returnPath);
};

#endif /* FOOTPATH_H */

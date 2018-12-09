#ifndef FOOT_TARGET_H
#define FOOT_TARGET_H

#include "../matrix/mat4.h"
#include "../matrix/vec2.h"
#include "../matrix/vec3.h"
#include "../mapping/World.h"
#include "../robot/Robot.h"

class FootTarget {
public:
    void control(Robot* robot, const World* world,
            const Mat4* avgFutureOrientation, const Mat4* onStepOrientation,
            const Vec3* footHome, Vec3* returnFootTarget);
};

#endif /* FOOT_TARGET_H */

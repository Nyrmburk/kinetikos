#ifndef MOTION_PLAN_H
#define MOTION_PLAN_H

#include "../matrix/mat4.h"

class MotionPlan {
public:
    virtual void orientationAt(float delta, Mat4* orientation) = 0;

    virtual void step(float delta) {
        time += delta;
    }

    float getTime() {
        return time;
    }
private:
    float time = 0;
};

#endif /* MOTION_PLAN_H */

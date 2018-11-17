#ifndef FOOT_H
#define FOOT_H

#include "../helper/Serializable.h"
#include "../tween/Bezier3Channel.h"

class Robot;

class FootControl {
public:
    // get foot from foot path
    void control(Robot* robot, const Bezier3Channel* path, float time, Vec3* returnFoot);
};

#endif /* FOOT_H */

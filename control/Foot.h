#ifndef FOOT_H
#define FOOT_H

#include "../helper/Serializable.h"
#include "../tween/Vec3Channel.h"

class Robot;

class Foot : public Serializable {
public:
    Vec3 foot = {0};

    void serialize(DataView* writer) {
        writer->writeFloat(foot.x);
        writer->writeFloat(foot.y);
        writer->writeFloat(foot.z);
    }

    void deserialize(DataView* reader) {
        foot.x = reader->readFloat();
        foot.y = reader->readFloat();
        foot.z = reader->readFloat();
    }
};

class FootControl {
public:
    // get foot from foot path
    void control(Robot* robot, const Vec3Channel* path, float time, Foot* returnFoot);
};

#endif /* FOOT_H */

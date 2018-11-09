#ifndef JOINTS_H
#define JOINTS_H

#include "../helper/Serializable.h"
#include "Foot.h"

class Robot;

class Joints : public Serializable {
public:
    float joints[3] = {0};

    void serialize(DataView* writer) {
        writer->writeFloats(joints, 3);
    }

    void deserialize(DataView* reader) {
        reader->readFloats(joints, 3);
    }
};

class JointsControl {
public:
    // compute the values for joints from the foot
    void control(Robot* robot, const Foot* foot, Joints* returnJoints);
};

#endif /* JOINTS_H */

#ifndef JOINTS_H
#define JOINTS_H

#include "../matrix/vec3.h"
#include "../helper/Serializable.h"

class Robot;

class Joints : public Serializable {
public:
    float coxa = 0;
    float femur = 0;
    float tibia = 0;

    void serialize(DataView* writer) {
        writer->writeFloat(coxa);
        writer->writeFloat(femur);
        writer->writeFloat(tibia);
    }

    void deserialize(DataView* reader) {
        coxa = reader->readFloat();
        femur = reader->readFloat();
        tibia = reader->readFloat();
    }
};

class JointsControl {
public:
    // compute the values for joints from the foot
    void control(Robot* robot, const Vec3* foot, Joints* returnJoints);
};

#endif /* JOINTS_H */

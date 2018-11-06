#ifndef JOINTS_H
#define JOINTS_H

#include "../helper/Serializable.h"

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

#endif /* JOINTS_H */

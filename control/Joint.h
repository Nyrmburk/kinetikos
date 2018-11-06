#ifndef JOINT_H
#define JOINT_H

#include "../helper/Serializable.h"

class Joint : public Serializable {
public:
    float joint = 0;

    void serialize(DataView* writer) {
        writer->writeFloat(joint);
    }

    void deserialize(DataView* reader) {
        joint = reader->readFloat();
    }
};

#endif /* JOINT_H */

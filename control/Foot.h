#ifndef FOOT_H
#define FOOT_H

#include "../helper/Serializable.h"

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

#endif /* FOOT_H */

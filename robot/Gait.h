#ifndef GAIT_H
#define GAIT_H

#include <math.h>

class Gait {
public:
    float strike;
    float duration;

    bool isGrounded(float cursor) {
        cursor = fmod(cursor, 1);
        return strike < cursor && (cursor - strike) < duration;
    }
};

#endif /* GAIT_H */

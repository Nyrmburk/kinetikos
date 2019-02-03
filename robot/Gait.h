#ifndef GAIT_H
#define GAIT_H

#include <math.h>

class Gait {
public:
    float strike;
    float duration;

    bool isGrounded(float cursor) {
        cursor = fmod(cursor + strike, 1);
        return 0 <= cursor && cursor < duration;
    }
};

#endif /* GAIT_H */

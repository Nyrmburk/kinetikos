#ifndef MAT4_CHANNEL_H
#define MAT4_CHANNEL_H

#include "../matrix/mat4.h"

class Mat4Channel : public TweenChannel<Mat4, Mat4> {
public:
    void ease(Mat4* start, Mat4* end, float k, Mat4* result) const {
        subm4(end, start, result);
        multm4s(result, k, result);
        addm4(start, result, result);
    }
};

#endif /*  MAT4_CHANNEL_H */

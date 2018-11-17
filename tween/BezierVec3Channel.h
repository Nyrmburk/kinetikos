#ifndef BEZIER_VEC3_CHANNEL_H
#define BEZIER_VEC3_CHANNEL_H

#include "TweenChannel.h"
#include "../curve/Bezier3.h"
#include "../matrix/vec3.h"

class BezierVec3Channel : public TweenChannel<Bezier3::Node, Vec3> {
public:
    void ease(Bezier3::Node* start, Bezier3::Node* end, float k, Vec3* result) const {
        Bezier3 curve(start, end, 10);
        curve.approximate();
        curve.linearValueAt(k, result);
    }
};

#endif /* BEZIER_VEC3_CHANNEL_H */


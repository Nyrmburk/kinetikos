#include "FootPath.h"
#include "../matrix/vec3.h"
#include "../animation/Tween.h"

void FootPathControl::control(Robot* robot,
        float sourceTime, const Vec3* source,
        float targetTime, const Vec3* target,
        Bezier3Channel* returnPath) {
    // make the leg lift up height equal to the distance traveled
    float distance = distancev3(source, target);
    Vec3 handle;
    setv3s(0, 0, distance, &handle);

    // create the start tween
    Bezier3::Node& start = *new Bezier3::Node;
    setv3(&start.point, source);
    setv3(&start.handle, &handle);
    Tween<Bezier3::Node> startTween(start, sourceTime, easeQuadraticInOut);

    // create the end tween
    Bezier3::Node& end = *new Bezier3::Node;
    setv3(&end.point, target);
    setv3(&end.handle, &handle);
    Tween<Bezier3::Node> endTween(end, targetTime, easeNone);

    // insert them into the current animation
    returnPath->insertTween(startTween);
    returnPath->insertTween(endTween);
}

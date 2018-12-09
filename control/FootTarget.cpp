#include "FootTarget.h"

void FootTarget::control(Robot* robot, const World* world,
        const Mat4* avgFutureOrientation, const Mat4* onStepOrientation,
        const Vec3* footHome, Vec3* returnFootTarget) {
    // convert foot home to world space
    Vec3 footHomeWorldSpace;
    multm4v3(avgFutureOrientation, footHome, 1, &footHomeWorldSpace);

    // do some calculation to find a good target...
    Vec3 footTarget;
    setv3(&footTarget, &footHomeWorldSpace);

    // stick target to floor
    // TODO: use raycasting when world is updated
    footTarget.z = 0;

    // calculate the orientation inverse so that the target can be converted to robot space
    Mat4 orientationInverse;
    inversem4(onStepOrientation, &orientationInverse);

    // convert target to robot space
    multm4v3(&orientationInverse, &footTarget, 1, returnFootTarget);
}


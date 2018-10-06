/* 
 * File:   leg.cpp
 * Author: nyrmburk
 * 
 * Created on June 23, 2017, 2:24 PM
 */

#include <math.h>
#include "helper/Math.h"
#include "matrix/transform.h"
#include "Leg.h"

Leg::Leg() {
}

void Leg::solveForward(const Mat4 *orientation, float joints[], const Vec3 *foot) {

    Mat4 matrix;

    identitym4(&matrix);
    setm4(&matrix, orientation);

    translate(&matrix, &offset);

    coxa.solve(&COXA_AXIS, joints[0], &matrix);
    femur.solve(&FEMUR_AXIS, joints[1], &matrix);
    tibia.solve(&TIBIA_AXIS, joints[2], &matrix);

    setPosition(&matrix, foot);
}

int Leg::solveInverse(const Mat4 *orientation, const Vec3 *foot, float joints[], const Vec3 *forward) {

    float *coxaJoint = &joints[0];
    float *femurJoint = &joints[1];
    float *tibiaJoint = &joints[2];

    Mat4 matrix;
    setm4(&matrix, orientation);
    Mat4 inverse;
    Vec3 relativeFoot;

    // translate offset away
    translate(&matrix, &offset);
    inversem4(&matrix, &inverse);
    multm4v3(&inverse, foot, 1, &relativeFoot);

    // calculate coxa angle
    *coxaJoint = atan2(relativeFoot.y, relativeFoot.x);

    // transform coxa away
    rotate(&matrix, &COXA_AXIS, *coxaJoint);
    Vec3 coxaTranslation = {coxa.length, 0, 0};
    translate(&matrix, &coxaTranslation);
    inversem4(&matrix, &inverse);
    multm4v3(&inverse, foot, 1, &relativeFoot);

    // now it is just a planar 2R arm
    float px = relativeFoot.x;
    float pz = relativeFoot.z;
    float pxx = px * px;
    float pzz = pz * pz;

    float l1 = femur.length;
    float l2 = tibia.length;
    float ll1 = l1 * l1;
    float ll2 = l2 * l2;

    float c2 = (pxx + pzz - ll1 - ll2) / (2 * l1 * l2);
    float s2 = -sqrt(1 - c2 * c2);

    // calculate femur angle
    *femurJoint = atan2(pz, px) - atan2(l2 * s2, l1 + l2 * c2);

    // calculate tibia angle
    *tibiaJoint = atan2(s2, c2);

    // normalize joints to their proper angles
    *coxaJoint = angleDifference(coxa.angle, *coxaJoint);
    *coxaJoint /= coxa.range;
    *coxaJoint += 0.5f;
    *coxaJoint = clamp(*coxaJoint, 0, 1);

    *femurJoint = angleDifference(femur.angle, *femurJoint);
    *femurJoint /= femur.range;
    *femurJoint += 0.5f;
    *femurJoint = clamp(*femurJoint, 0, 1);

    *tibiaJoint = angleDifference(tibia.angle, *tibiaJoint);
    *tibiaJoint /= tibia.range;
    *tibiaJoint += 0.5f;
    *tibiaJoint = clamp(*tibiaJoint, 0, 1);

    if (forward) {
        solveForward(orientation, joints, forward);
        return distancev3(foot, forward) < 0.01;
    }
    return 1;
}

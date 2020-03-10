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

void Leg::solveForward(const Mat4 *orientation, const Joints* joints, Vec3 *foot) {

    Mat4 matrix;

    identitym4(&matrix);
    setm4(&matrix, orientation);

    translate(&matrix, &offset);

    coxa.solve(&COXA_AXIS, joints->coxa, &matrix);
    femur.solve(&FEMUR_AXIS, joints->femur, &matrix);
    tibia.solve(&TIBIA_AXIS, joints->tibia, &matrix);

    getPosition(&matrix, foot);
}

int Leg::solveInverse(const Mat4 *orientation, const Vec3 *foot, Joints* joints, Vec3 *forward) {

    Mat4 matrix;
    setm4(&matrix, orientation);
    Mat4 inverse;
    Vec3 relativeFoot;

    // translate offset away
    translate(&matrix, &offset);
    inversem4(&matrix, &inverse);
    multm4v3(&inverse, foot, 1, &relativeFoot);

    // calculate coxa angle
    joints->coxa = atan2(relativeFoot.y, relativeFoot.x);

    // transform coxa away
    rotate(&matrix, &COXA_AXIS, joints->coxa);
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
    joints->femur = atan2(pz, px) - atan2(l2 * s2, l1 + l2 * c2);

    // calculate tibia angle
    joints->tibia = atan2(s2, c2);

    // normalize joints to their proper angles
    joints->coxa = angleDifference(coxa.angle, joints->coxa);
    joints->coxa /= coxa.range;
    joints->coxa += 0.5f;
    joints->coxa = clamp(joints->coxa, 0, 1);

    joints->femur = angleDifference(femur.angle, joints->femur);
    joints->femur /= femur.range;
    joints->femur += 0.5f;
    joints->femur = clamp(joints->femur, 0, 1);

    joints->tibia = angleDifference(tibia.angle, joints->tibia);
    joints->tibia /= tibia.range;
    joints->tibia += 0.5f;
    joints->tibia = clamp(joints->tibia, 0, 1);

    if (forward) {
        solveForward(orientation, joints, forward);
        return distancev3(foot, forward) < 0.01;
    }
    return 1;
}

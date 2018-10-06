/*
 * File:   bone.cpp
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 2:24 PM
 */

#include "Bone.h"

#include "../matrix/transform.h"

Bone::Bone() {
}

void Bone::solve(const Vec3 *axis, const float joint, Mat4 *matrix) {

    Vec3 translation = {length, 0, 0};
    rotate(matrix, axis, angle - range * (joint - 0.5));
    translate(matrix, &translation);
}

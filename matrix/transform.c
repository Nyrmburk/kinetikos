/*
 * Copyright (C) 2017 nyrmburk
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <math.h>
#include "transform.h"

void getPosition(const Mat4 *matrix, Vec3 *result) {

    result->x = matrix->m[12] / matrix->m[15];
    result->y = matrix->m[13] / matrix->m[15];
    result->z = matrix->m[14] / matrix->m[15];
}

void setPosition(Mat4 *matrix, const Vec3 *position) {

    matrix->m[12] = position->x * matrix->m[15];
    matrix->m[13] = position->y * matrix->m[15];
    matrix->m[14] = position->z * matrix->m[15];
}

void translate(Mat4 *matrix, Vec3 *translation) {

    Mat4 identity;
    identitym4(&identity);
    setPosition(&identity, translation);

    Mat4 result;
    
    multm4m4(matrix, &identity, &result);
    
    setm4(matrix, &result);
}

//mat3* getRotationMatrix(mat4 *matrix, mat3 *result) {
//
//    result.m[0] = matrix.m[0];
//    result.m[1] = matrix.m[1];
//    result.m[2] = matrix.m[2];
//
//    result.m[3] = matrix.m[4];
//    result.m[4] = matrix.m[5];
//    result.m[5] = matrix.m[6];
//
//    result.m[6] = matrix.m[8];
//    result.m[7] = matrix.m[9];
//    result.m[8] = matrix.m[10];
//}
//
//void setRotationMatrix(mat4 *matrix, mat3 *rotation);
//
//vec4* getRotationQuaternion(mat4 *matrix, vec4 *result);
//
//void setRotationQuaternion(mat4 *matrix, vec4 *quaternion);

void rotate(Mat4 *matrix, const Vec3 *axis, const float angle) {

    Mat4 rotation;
    identitym4(&rotation);
    
    float c = cos(angle);
    float s = sin(angle);
    float t = 1 - c;
    float x = axis->x, y = axis->y, z = axis->z;
    float tx = t * x, ty = t * y;
    
    rotation.m[0] = tx * x + c;
    rotation.m[4] = tx * y - s * z;
    rotation.m[8] = tx * z + s * y;
    
    rotation.m[1] = tx * y + s * z;
    rotation.m[5] = ty * y + c;
    rotation.m[9] = ty * z - s * x;
    
    rotation.m[2] = tx * z - s * y;
    rotation.m[6] = ty * z + s * x;
    rotation.m[10] = t * z * z + c;
    
    Mat4 result;
    multm4m4(matrix, &rotation, &result);
    setm4(matrix, &result);

//    float xx = axis->x * axis->x;
//    float xy = axis->x * axis->y;
//    float xz = axis->x * axis->z;
//    float yy = axis->y * axis->y;
//    float yz = axis->y * axis->z;
//    float zz = axis->z * axis->z;
//
//    float sinval = sin(angle);
//    float cosval = cos(angle);
//    float one_minus_cos = 1 - cosval;
//
//    matrix->m[0] = xx * one_minus_cos + cosval;
//    matrix->m[1] = xy * one_minus_cos + axis->z * sinval;
//    matrix->m[2] = xz * one_minus_cos - axis->y * sinval;
//    matrix->m[4] = xy * one_minus_cos - axis->z * sinval;
//    matrix->m[5] = yy * one_minus_cos + cosval;
//    matrix->m[6] = yz * one_minus_cos + axis->x * sinval;
//    matrix->m[8] = xz * one_minus_cos + axis->y * sinval;
//    matrix->m[9] = yz * one_minus_cos - axis->x * sinval;
//    matrix->m[10]= zz * one_minus_cos + cosval;
}

void scale(Mat4 *matrix, const float scale) {
    matrix->m[15] /= scale;
}

void lookat(Mat4 *matrix, Vec3 *position, Vec3 *target, Vec3 *up) {
    
}

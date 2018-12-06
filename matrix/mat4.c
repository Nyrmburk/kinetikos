/*
 * Copyright (C) 2016 nyrmburk
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

#include <stdlib.h>
#include "mat4.h"

void setm4(Mat4 *to, const Mat4 *from) {
    for (int i = 0; i < TOTAL_SIZE_M4; i++)
        to->m[i] = from->m[i];
}

void addm4(const Mat4 *from, const Mat4 *to, Mat4 *result) {
    for (int i = 0; i < TOTAL_SIZE_M4; i++)
        result->m[i] = from->m[i] + to->m[i];
}

void subm4(const Mat4 *from, const Mat4 *to, Mat4 *result) {
    for (int i = 0; i < TOTAL_SIZE_M4; i++)
        result->m[i] = from->m[i] - to->m[i];
}

void multm4s(const Mat4 *from, float to, Mat4 *result) {
    for (int i = 0; i < TOTAL_SIZE_M4; i++)
        result->m[i] = from->m[i] * to;
}

void multm4m4(const Mat4 *from, const Mat4 *to, Mat4 *result) {
    const float *f = from->m;
    const float *t = to->m;
    float *r = result->m;
    for (int i = 0; i < TOTAL_SIZE_M4; i++) {

        int row = i % SIZE_M4;
        int col = i / SIZE_M4 * SIZE_M4;

        float dot = 0;
        for (int j = 0; j < SIZE_M4; j++) {
            dot += f[row] * t[col++];
            row += SIZE_M4;
        }

        r[i] = dot;
    }
}

void multm4v4(const Mat4 *from, const Vec4 *to, Vec4 *result) {
    result->x = to->x * from->m[0] + to->y * from->m[4] + to->z * from->m[8] + to->w * from->m[12];
    result->y = to->x * from->m[1] + to->y * from->m[5] + to->z * from->m[9] + to->w * from->m[13];
    result->z = to->x * from->m[2] + to->y * from->m[6] + to->z * from->m[10] + to->w * from->m[14];
    result->w = to->x * from->m[3] + to->y * from->m[7] + to->z * from->m[11] + to->w * from->m[15];
}

float multm4v3(const Mat4 *from, const Vec3 *to, float w, Vec3 *result) {
    result->x = to->x * from->m[0] + to->y * from->m[4] + to->z * from->m[8] + w * from->m[12];
    result->y = to->x * from->m[1] + to->y * from->m[5] + to->z * from->m[9] + w * from->m[13];
    result->z = to->x * from->m[2] + to->y * from->m[6] + to->z * from->m[10] + w * from->m[14];
    w = to->x * from->m[3] + to->y * from->m[7] + to->z * from->m[11] + w * from->m[15];
    return w;
}

float dotm4(const Mat4 *from, const Mat4 *to) {
    float dot = 0;
    for (int i = 0; i < TOTAL_SIZE_M4; i++)
        dot += from->m[i] * to->m[i];
    return dot;
}

void transposem4(const Mat4 *from, Mat4 *result) {
    float *m = result->m;
    float temp;

    temp = from->m[1];
    m[1] = from->m[4];
    m[4] = temp;

    temp = from->m[2];
    m[2] = from->m[8];
    m[8] = temp;

    temp = from->m[3];
    m[3] = from->m[12];
    m[12] = temp;

    temp = from->m[6];
    m[6] = from->m[9];
    m[9] = temp;

    temp = from->m[7];
    m[7] = from->m[13];
    m[13] = temp;

    temp = from->m[11];
    m[11] = from->m[14];
    m[14] = temp;
}

void tracem4(const Mat4 *from, const Mat4 *to, Mat4 *result);

float determinantm4(const Mat4 *matrix) {
    
    const float *m = matrix->m;
    float determinant = 0;

    determinant += m[0] * m[5] * m[10] * m[15] - m[0] * m[5] * m[14] * m[11];
    determinant += m[0] * m[9] * m[14] * m[7] - m[0] * m[9] * m[6] * m[15];
    determinant += m[0] * m[13] * m[6] * m[11] - m[0] * m[13] * m[10] * m[7];

    determinant += m[4] * m[1] * m[14] * m[11] - m[4] * m[1] * m[10] * m[15];
    determinant += m[4] * m[9] * m[2] * m[15] - m[4] * m[9] * m[14] * m[3];
    determinant += m[4] * m[13] * m[10] * m[3] - m[4] * m[13] * m[2] * m[11];

    determinant += m[8] * m[1] * m[6] * m[15] - m[8] * m[1] * m[14] * m[7];
    determinant += m[8] * m[5] * m[14] * m[3] - m[8] * m[5] * m[2] * m[15];
    determinant += m[8] * m[13] * m[2] * m[7] - m[8] * m[13] * m[6] * m[3];

    determinant += m[12] * m[1] * m[10] * m[7] - m[12] * m[1] * m[6] * m[11];
    determinant += m[12] * m[5] * m[2] * m[11] - m[12] * m[5] * m[10] * m[3];
    determinant += m[12] * m[9] * m[6] * m[3] - m[12] * m[9] * m[2] * m[7];

    return determinant;
}

float inversem4(const Mat4 *matrix, Mat4 *result) {
    
    float determinant = determinantm4(matrix);

    // inverse does not exist when determinant is 0
    if (determinant == 0) {
        return 0;
    }

    const float *m = matrix->m;
    float *r = result->m;

    r[0] = m[5] * m[10] * m[15] + m[9] * m[14] * m[7] + m[13] * m[6] * m[11];
    r[0] -= m[5] * m[14] * m[11] + m[9] * m[6] * m[15] + m[13] * m[10] * m[7];
    r[0] /= determinant;

    r[4] = m[4] * m[14] * m[11] + m[8] * m[6] * m[15] + m[12] * m[10] * m[7];
    r[4] -= m[4] * m[10] * m[15] + m[8] * m[14] * m[7] + m[12] * m[6] * m[11];
    r[4] /= determinant;

    r[8] = m[4] * m[9] * m[15] + m[8] * m[13] * m[7] + m[12] * m[5] * m[11];
    r[8] -= m[4] * m[13] * m[11] + m[8] * m[5] * m[15] + m[12] * m[9] * m[7];
    r[8] /= determinant;

    r[12] = m[4] * m[13] * m[10] + m[8] * m[5] * m[14] + m[12] * m[9] * m[6];
    r[12] -= m[4] * m[9] * m[14] + m[8] * m[13] * m[6] + m[12] * m[5] * m[10];
    r[12] /= determinant;

    r[1] = m[1] * m[14] * m[11] + m[9] * m[2] * m[15] + m[13] * m[10] * m[3];
    r[1] -= m[1] * m[10] * m[15] + m[9] * m[14] * m[3] + m[13] * m[2] * m[11];
    r[1] /= determinant;

    r[5] = m[0] * m[10] * m[15] + m[8] * m[14] * m[3] + m[12] * m[2] * m[11];
    r[5] -= m[0] * m[14] * m[11] + m[8] * m[2] * m[15] + m[12] * m[10] * m[3];
    r[5] /= determinant;

    r[9] = m[0] * m[13] * m[11] + m[8] * m[1] * m[15] + m[12] * m[9] * m[3];
    r[9] -= m[0] * m[9] * m[15] + m[8] * m[13] * m[3] + m[12] * m[1] * m[11];
    r[9] /= determinant;

    r[13] = m[0] * m[9] * m[14] + m[8] * m[13] * m[2] + m[12] * m[1] * m[10];
    r[13] -= m[0] * m[13] * m[10] + m[8] * m[1] * m[14] + m[12] * m[9] * m[2];
    r[13] /= determinant;

    r[2] = m[1] * m[6] * m[15] + m[5] * m[14] * m[3] + m[13] * m[2] * m[7];
    r[2] -= m[1] * m[14] * m[7] + m[5] * m[2] * m[15] + m[13] * m[6] * m[3];
    r[2] /= determinant;

    r[6] = m[0] * m[14] * m[7] + m[4] * m[2] * m[15] + m[12] * m[6] * m[3];
    r[6] -= m[0] * m[6] * m[15] + m[4] * m[14] * m[3] + m[12] * m[2] * m[7];
    r[6] /= determinant;

    r[10] = m[0] * m[5] * m[15] + m[4] * m[13] * m[3] + m[12] * m[1] * m[7];
    r[10] -= m[0] * m[13] * m[7] + m[4] * m[1] * m[15] + m[12] * m[5] * m[3];
    r[10] /= determinant;

    r[14] = m[0] * m[13] * m[6] + m[4] * m[1] * m[14] + m[12] * m[5] * m[2];
    r[14] -= m[0] * m[5] * m[14] + m[4] * m[13] * m[2] + m[12] * m[1] * m[6];
    r[14] /= determinant;

    r[3] = m[1] * m[10] * m[7] + m[5] * m[2] * m[11] + m[9] * m[6] * m[3];
    r[3] -= m[1] * m[6] * m[11] + m[5] * m[10] * m[3] + m[9] * m[2] * m[7];
    r[3] /= determinant;

    r[7] = m[0] * m[6] * m[11] + m[4] * m[10] * m[3] + m[8] * m[2] * m[7];
    r[7] -= m[0] * m[10] * m[7] + m[4] * m[2] * m[11] + m[8] * m[6] * m[3];
    r[7] /= determinant;

    r[11] = m[0] * m[9] * m[7] + m[4] * m[1] * m[11] + m[8] * m[5] * m[3];
    r[11] -= m[0] * m[5] * m[11] + m[4] * m[9] * m[3] + m[8] * m[1] * m[7];
    r[11] /= determinant;

    r[15] = m[0] * m[5] * m[10] + m[4] * m[9] * m[2] + m[8] * m[1] * m[6];
    r[15] -= m[0] * m[9] * m[6] + m[4] * m[1] * m[10] + m[8] * m[5] * m[2];
    r[15] /= determinant;

    return determinant;
}

void addm4(const Mat4 *from, const Mat4 *to, Mat4 *result);

void identitym4(Mat4 *matrix) {
    
    matrix->m[0] = 1;
    matrix->m[1] = 0;
    matrix->m[2] = 0;
    matrix->m[3] = 0;
    
    matrix->m[4] = 0;
    matrix->m[5] = 1;
    matrix->m[6] = 0;
    matrix->m[7] = 0;
    
    matrix->m[8] = 0;
    matrix->m[9] = 0;
    matrix->m[10] = 1;
    matrix->m[11] = 0;
    
    matrix->m[12] = 0;
    matrix->m[13] = 0;
    matrix->m[14] = 0;
    matrix->m[15] = 1;
}

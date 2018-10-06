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

/* 
 * File:   Mat4.h
 * Author: nyrmburk
 *
 * Created on November 2, 2016, 8:28 AM
 */

#ifndef MAT4_H
#define MAT4_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "vec4.h"
#include "vec3.h"
    
#define SIZE_M4 4
#define TOTAL_SIZE_M4 16

    typedef struct {
        float m[TOTAL_SIZE_M4];
    } Mat4;
    
    void setm4(Mat4 *to, const Mat4 *from);
    void addm4(const Mat4 *from, const Mat4 *to, Mat4 *result);
    void subm4(const Mat4 *from, const Mat4 *to, Mat4 *result);
    void multm4s(const Mat4 *from, float to, Mat4 *result);
    void multm4m4(const Mat4 *from, const Mat4 *to, Mat4 *result);
    void multm4v4(const Mat4 *from, const Vec4 *to, Vec4 *result);
    float multm4v3(const Mat4 *from, const Vec3 *to, float w, Vec3 *result);
    float dotm4(const Mat4 *from, const Mat4 *to);
    void transposem4(const Mat4 *from, const Mat4 *to, Mat4 *result);
    void tracem4(const Mat4 *from, const Mat4 *to, Mat4 *result);
    float determinantm4(const Mat4 *matrix);
    float inversem4(const Mat4 *matrix, Mat4 *result);
      
    void identitym4(Mat4 *matrix);

#ifdef __cplusplus
}
#endif

#endif /* MAT4_H */


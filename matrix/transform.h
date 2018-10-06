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

/* 
 * File:   transform.h
 * Author: nyrmburk
 *
 * Created on April 26, 2017, 5:41 PM
 */

#ifndef TRANSFORM_H
#define TRANSFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vec3.h"
#include "mat4.h"

	//  0  4  8 12
	//  1  5  9 13
	//  2  6 10 14
	//  3  7 11 15

	//  r  r  r  x
	//  r  r  r  y
	//  r  r  r  z
	//  ?  ?  ?  w?

    void getPosition(const Mat4 *matrix, Vec3 *result);
    
    void setPosition(Mat4 *matrix, const Vec3 *position);
    
    void translate(Mat4 *matrix, Vec3 *translation);
    
//    mat3* getRotationMatrix(mat4 *matrix, mat3 *result);
    
//    void setRotationMatrix(mat4 *matrix, mat3 *rotation);
    
//    vec4* getRotationQuaternion(mat4 *matrix, vec4 *result);
    
//    void setRotationQuaternion(mat4 *matrix, vec4 *quaternion);
    
    void rotate(Mat4 *matrix, const Vec3 *axis, const float angle);
    
    void scale(Mat4 *matrix, const float scale);
    
    void lookat(Mat4 *matrix, Vec3 *position, Vec3 *target, Vec3 *up);

#ifdef __cplusplus
}
#endif

#endif /* TRANSFORM_H */


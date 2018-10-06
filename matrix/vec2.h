/*
 * Copyright (C) 2017 nyrmburk
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
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
 * File:   vec2.h
 * Author: nyrmburk
 *
 * Created on May 2, 2017, 2:09 PM
 */

#ifndef VEC2_H
#define VEC2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vec3.h"

#define SIZE_V2 2

typedef struct {
    float x, y;
} Vec2;

void addv2(Vec2 *from, Vec2 *toAdd, Vec2 *result);
void subtractv2(Vec2 *from, Vec2 *toSub, Vec2 *result);
void multiplyv2v2(Vec2 *from, Vec2 *toMult, Vec2 *result);
void multiplyv2s(Vec2 *from, float scalar, Vec2 *result);
void dividev2v2(Vec2 *from, Vec2 *toDiv, Vec2 *result);
void dividev2s(Vec2 *from, float scalar, Vec2 *result);
float dotv2(Vec2 *from, Vec2 *toDot);
void crossv2(Vec2 *from, Vec2 *toCross, Vec2 *result);
float anglev2(Vec2 *from, Vec2 *toCompare);
void reciprocalv2(Vec2 *vec, Vec2 *result);
void negatev2(Vec2 *vec, Vec2 *result);
float normalizedv2(Vec2 *vec, Vec2 *result);
void translatev2(Vec2 *from, Vec2 *delta);
void rotatev2(Vec2 *from, Vec2 *center, float radians);
void scalev2(Vec2 *from, Vec2 *center, float amount);
float distancev2(Vec2 *from, Vec2 *vec);
float lengthv2(Vec2 *vec);
void setv2(Vec2 *from, Vec2 *vec);
int equalsv2(Vec2 *from, Vec2 *vec);

Vec3* v2asv3(Vec2 *from, float z, Vec3 *result);


#ifdef __cplusplus
}
#endif

#endif /* VEC2_H */


/*
 * Copyright v3(C) 2016 nyrmburk
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * v3(at your option) any later version.
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
 * File:   vec3.h
 * Author: nyrmburk
 *
 * Created on November 2, 2016, 8:28 AM
 */

#ifndef VEC3_H
#define VEC3_H

#ifdef __cplusplus
extern "C" {
#endif
    
#define SIZE_V3 3

typedef struct {
    float x, y, z;
} Vec3;

void addv3(const Vec3 *from, const Vec3 *toAdd, Vec3 *result);
void subtractv3(const Vec3 *from, const Vec3 *toSub, Vec3 *result);
void multiplyv3v3(const Vec3 *from, const Vec3 *toMult, Vec3 *result);
void multiplyv3s(const Vec3 *from, float scalar, Vec3 *result);
void dividev3v3(const Vec3 *from, const Vec3 *toDiv, Vec3 *result);
void dividev3s(const Vec3 *from, float scalar, Vec3 *result);
float dotv3(const Vec3 *from, const Vec3 *toDot);
void crossv3(const Vec3 *from, const Vec3 *toCross, Vec3 *result);
float anglev3(const Vec3 *from, const Vec3 *toCompare);
void reciprocalv3(const Vec3 *vec, Vec3 *result);
void negatev3(const Vec3 *vec, Vec3 *result);
float normalizedv3(const Vec3 *vec, Vec3 *result);
void translatev3(Vec3 *from, Vec3 *delta);
void rotatev3(Vec3 *from, Vec3 *center, float radians);
void scalev3(Vec3 *from, Vec3 *center, float amount);
float distancev3(const Vec3 *from, const Vec3 *to);
float lengthv3(const Vec3 *vec);
float mannhattanv3(const Vec3 *vec);
void setv3(Vec3 *to, const Vec3 *from);
int equalsv3(const Vec3 *from, const Vec3 *vec);

#ifdef __cplusplus
}
#endif

#endif /* VEC3_H */


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
#include "vec3.h"

void addv3(const Vec3 *from, const Vec3 *toAdd, Vec3 *result) {
    result->x = from->x + toAdd->x;
    result->y = from->y + toAdd->y;
    result->z = from->z + toAdd->z;
}

void subtractv3(const Vec3 *from, const Vec3 *toSub, Vec3 *result) {
    result->x = from->x - toSub->x;
    result->y = from->y - toSub->y;
    result->z = from->z - toSub->z;
}

void multiplyv3v3(const Vec3 *from, const Vec3 *toMult, Vec3 *result) {
    result->x = from->x * toMult->x;
    result->y = from->y * toMult->y;
    result->z = from->z * toMult->z;
}

void multiplyv3s(const Vec3 *from, float scalar, Vec3 *result) {
    result->x = from->x * scalar;
    result->y = from->y * scalar;
    result->z = from->z * scalar;
}

void dividev3v3(const Vec3 *from, const Vec3 *toDiv, Vec3 *result);

void dividev3s(const Vec3 *from, float scalar, Vec3 *result) {
    result->x = from->x / scalar;
    result->y = from->y / scalar;
    result->z = from->z / scalar;
}

float dotv3(const Vec3 *from, const Vec3 *toDot);
void crossv3(const Vec3 *from, const Vec3 *toCross, Vec3 *result);
float anglev3(const Vec3 *from, const Vec3 *toCompare);
void reciprocalv3(const Vec3 *vec, Vec3 *result);

void negatev3(const Vec3 *vec, Vec3 *result) {
    result->x = -vec->x;
    result->y = -vec->y;
    result->z = -vec->z;
}

float normalizedv3(const Vec3 *vec, Vec3 *result) {
    float length = lengthv3(vec);
    if (length != 0) {
        dividev3s(vec, length, result);
    } else {
        result->x = 0;
        result->y = 0;
        result->z = 0;
    }
    return length;
}

void translatev3(Vec3 *from, Vec3 *delta);
void rotatev3(Vec3 *from, Vec3 *center, float radians);
void scalev3(Vec3 *from, Vec3 *center, float amount);

float distancev3(const Vec3 *from, const Vec3 *to) {
    float x = to->x - from->x;
    float y = to->y - from->y;
    float z = to->z - from->z;
    return sqrt(x * x + y * y + z * z);
}

float lengthv3(const Vec3 *vec) {
    return sqrt(vec->x * vec->x + vec->y * vec->y + vec->z + vec->z);
}

float mannhattanv3(const Vec3 *vec) {
    return vec->x + vec->y + vec->z;
}


void setv3(Vec3 *from, const Vec3 *vec) {
    from->x = vec->x;
    from->y = vec->y;
    from->z = vec->z;
}

int equalsv3(const Vec3 *from, const Vec3 *vec) {
    return from->x == vec->x && from->y == vec->y && from->z == vec->z;
}

//public Vec3 add(Vec3 toAdd) {
//
//        return new Vec3(x + toAdd.x, y + toAdd.y, z + toAdd.z);
//}
//
//public Vec3 subtract(Vec3 toSub) {
//
//        return new Vec3(x - toSub.x, y - toSub.y, z - toSub.z);
//}
//
//public Vec3 multiply(Vec3 toMult) {
//
//        return new Vec3(x * toMult.x, y * toMult.y, z * toMult.z);
//}
//
//public Vec3 multiply(float scalar) {
//
//        return new Vec3(x * scalar, y * scalar, z * scalar);
//}
//
//public Vec3 divide(Vec3 toDiv) {
//
//        return new Vec3(x / toDiv.x, y / toDiv.y, z / toDiv.z);
//}
//
//public Vec3 divide(float scalar) {
//
//        return new Vec3(x / scalar, y / scalar, z / scalar);
//}
//
//public float dot(Vec3 toDot) {
//
//        return x * toDot.x + this.y * toDot.y + z * toDot.z;
//}
//
//public Vec3 cross(Vec3 toCross) {
//
//        Vec3 crossed = new Vec3();
//
//        crossed.x = y * toCross.z - z * toCross.y;
//        crossed.y = z * toCross.x - x * toCross.z;
//        crossed.z = x * toCross.y - y * toCross.x;
//
//        return crossed;
//}
//
//public float angle(Vec3 toCompare) {
//
//        return (float) Math.acos(this.dot(toCompare) / (this.length() * toCompare.length()));
//}
//
//public Vec3 reciprocal() {
//
//        return new Vec3(1 / x, 1 / y, 1 / z);
//}
//
//public Vec3 negate() {
//
//        return new Vec3(-x, -y, -z);
//}
//
//public Vec3 normalized() {
//
//        return new Vec3(this.divide(length()));
//}
//
//
//public void translate(Vec3 delta) {
//
//        set(this.add(delta));
//}
//
////TODO needs axis
////	public void rotate(Vec3 center, float radians) {
////
////		double angle = Math.atan2(y - center.getY(), x - center.getX());
////		angle += radians;
////
////		float length = this.length(center);
////
////		float x = (float) Math.cos(angle) * length + center.getX();
////		float y = (float) Math.sin(angle) * length + center.getY();
////
////		set(new Vec3(x, y, z));
////	}
//
//public void scale(Vec3 center, float amount) {
//
//        Vec3 delta = this.subtract(center).normalized();
//        delta.multiply(amount);
//
//        set(delta.add(center));
//}
//
//public float distance(Vec3 vec) {
//
//        return vec.subtract(this).length();
//}
//
//public float length() {
//
//        return (float) Math.sqrt(x * x + y * y + z * z);
//}
//
//public void set(Vec3 vec) {
//
//        x = vec.x;
//        y = vec.y;
//        z = vec.z;
//}
//
//public Vec4 asVec4(float w) {
//
//        return new Vec4(x, y, z, w);
//}
//
//public Vec4 asVec4() {
//
//        return asVec4(0);
//}
//
//@Override
//public boolean equals(Object obj) {
//
//        if (obj instanceof Vec3) {
//
//                Vec3 vec = (Vec3) obj;
//                if (x == vec.x && y == vec.y && z == vec.z)
//                        return true;
//        }
//
//        return false;


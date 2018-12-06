/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   leg.h
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 2:24 PM
 */

#ifndef LEG_H
#define LEG_H

#include "Bone.h"
#include "../helper/Serializable.h"
#include "../matrix/vec3.h"
#include "../matrix/MatSerialize.h"
#include "../control/Joints.h"

#define BONES_PER_LEG 3

const Vec3 COXA_AXIS = {0, 0, 1}; // z is up
const Vec3 FEMUR_AXIS = {0, -1, 0}; // y is forward
const Vec3 TIBIA_AXIS = FEMUR_AXIS;

class Leg : public Serializable {
public:
    Leg();

    Vec3 offset; // could be mat4
    Bone coxa;
    Bone femur;
    Bone tibia;
    
    void solveForward(const Mat4 *orientation, const Joints* joints, Vec3 *foot);
    int solveInverse(const Mat4 *orientation, const Vec3 *foot, Joints* joints, Vec3 *forward);

    void serialize(DataView* data) {
        serializev3(data, &offset);
        data->writeSerial(&coxa);
        data->writeSerial(&femur);
        data->writeSerial(&tibia);
    }

    void deserialize(DataView* data) {
        deserializev3(data, &offset);
        data->readSerial(&coxa);
        data->readSerial(&femur);
        data->readSerial(&tibia);
    }
};

#endif /* LEG_H */


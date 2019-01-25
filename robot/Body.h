/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   body.h
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 2:27 PM
 */

#ifndef BODY_H
#define BODY_H

#include "Leg.h"
#include "../helper/Serializable.h"
#include "../matrix/mat4.h"

struct Body : public Serializable {
    uint8_t legsCount = 0;
    Leg *legs = nullptr;

    void serialize(DataView* data) {
        data->writeU8(legsCount);
        for (uint8_t i = 0; i < legsCount; i++) {
            data->writeSerial(&legs[i]);
        }
    }

    void deserialize(DataView* data) {
        legsCount = data->readU8();
        delete[] legs;
        legs = new Leg[legsCount];
        for (uint8_t i = 0; i < legsCount; i++) {
            data->readSerial(&legs[i]);
        }
    }
};

#endif /* BODY_H */


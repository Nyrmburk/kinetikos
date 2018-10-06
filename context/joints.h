/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   joints.h
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 10:43 PM
 */

#ifndef JOINTS_H
#define JOINTS_H

#include <stdlib.h>
#include <vector>
#include "../helper/publisher.h"

class joints: public publisher {
public:
    joints(size_t size);
    size_t size();
    float getJoint(size_t i);
    void setJoint(size_t i, float angle);
    virtual ~joints();
private:
    size_t m_size;
    float* m_joints;
};

#endif /* JOINTS_H */


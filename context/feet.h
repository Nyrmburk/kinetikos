/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   feet.h
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 11:47 PM
 */

#ifndef FEET_H
#define FEET_H

#include <stdlib.h>
#include <list>
#include "../matrix/vec3.h"

using namespace std;

class feet {
public:
    feet();
    virtual ~feet();
private:
    size_t m_size;
    Vec3* m_feet;
    // this actually needs to be a spline/path with speed and duration
    list<Vec3*>* targets; 
};

#endif /* FEET_H */

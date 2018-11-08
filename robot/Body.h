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

#include "../matrix/mat4.h"
#include "Leg.h"

struct Body {
    Mat4 orientation;
    int legsCount;
    Leg *legs;
};

#endif /* BODY_H */


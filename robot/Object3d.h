/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   object3d.h
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 2:24 PM
 */

#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <vector>
#include "../matrix/mat4.h"

class Object3d : public Mat4 {
public:
    Object3d();
    virtual ~Object3d();
    
    Object3d getParent();
    void putChild();
    void removeChild(int index);
    Object3d getChild(int index);
    int size();
private:
    Object3d* parent;
    std::vector<Object3d> children;
};

#endif /* OBJECT3D_H */


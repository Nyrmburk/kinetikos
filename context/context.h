/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   context.h
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 2:29 PM
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include "../robot/Body.h"
#include "joints.h"
#include "trim.h"

class context {
public:
    context();
    const Body* m_body;
    const trim* m_trim;
    const joints* m_joints;
    virtual ~context();
private:
};

#endif /* CONTEXT_H */

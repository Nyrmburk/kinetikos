/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   System.h
 * Author: nyrmburk
 *
 * Created on May 8, 2018, 8:34 PM
 */

#ifndef SYSTEM_H
#define SYSTEM_H

class Intent {
public:
    virtual void start() = 0;
    virtual void step(float delta) = 0;
    virtual void stop() = 0;
};

#endif /* SYSTEM_H */


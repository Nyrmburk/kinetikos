/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   gaitControl.h
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 2:21 PM
 */

#ifndef GAITCONTROL_H
#define GAITCONTROL_H

#include <string>

#include "../robot/Robot.h"

class Robot;

class GaitControl {
public:
    GaitControl();
    virtual ~GaitControl();
    
    struct Gait {
        std::string name;// = "alternating tripod";
        float duty;// = 0.6;
        float offset[];// = {0, 0.5, 0, 0.5, 0, 0.5};
    };
    Gait *gait;
    
    // is this a bad idea?
    // because it doesn't fit in with my whole "stack" idea
    // the pattern for this requires it to get the data by itself
    // but my stack idea allows for it to be controlled end-to-end
    void step(float delta);
    
private:
    Robot *robot;
    float cursor = 0;
};

#endif /* GAITCONTROL_H */


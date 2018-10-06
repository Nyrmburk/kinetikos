/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NullMotorControl.h
 * Author: nyrmburk
 *
 * Created on May 2, 2018, 11:44 PM
 */

#ifndef NULLMOTORCONTROL_H
#define NULLMOTORCONTROL_H

#include "MotorControl.h"

class NullMotorControl : public MotorControl {
    void setMotors(float joints[], int numJoints) {
        // do nothing :)
    }
};

#endif /* NULLMOTORCONTROL_H */


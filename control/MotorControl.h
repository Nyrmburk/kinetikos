/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   motor_control.h
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 2:22 PM
 */

#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "../robot/Leg.h"

class MotorControl {
public:
    virtual void setMotors(Joints* joints, int numJoints) = 0;
};

#endif /* MOTOR_CONTROL_H */


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PiPololuMotorControl.h
 * Author: nyrmburk
 *
 * Created on May 2, 2018, 11:49 PM
 */

#ifndef PIPOLOLUMOTORCONTROL_H
#define PIPOLOLUMOTORCONTROL_H

#include "MotorControl.h"

#define DEVICE "/dev/ttyAMA0"
#define SERIAL_TX 15
#define SERIAL_RX 16
#define BAUD 115200
#define MICROSECOND_PRECISION 4
    
// servo mins and maxes are defined in quarter-microseconds
#define SERVO_MIN 1000 * MICROSECOND_PRECISION // absolute min 64us
#define SERVO_MAX 2000 * MICROSECOND_PRECISION // absolute max 4080us

// TODO make constructor and destructor
// has to have a static fd and also be a singleton
// or maybe somehow fail on create using exceptions or some shit
    
class PiPololuMotorControl : public MotorControl {
public:
    PiPololuMotorControl(int fd);
    virtual ~PiPololuMotorControl();
    void writeServo(int servo, float angle) ;
    void setMotors(Joints* joints, int numJoints);
    static int getSerial();
private:
    int fd;
};

#endif /* PIPOLOLUMOTORCONTROL_H */


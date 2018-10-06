/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "PiPololuMotorControl.h"

PiPololuMotorControl::PiPololuMotorControl() throw (int) {
    
    wiringPiSetup();
    fd = serialOpen(DEVICE, BAUD);
    
    if (!fd) throw 1;
    
    // clears the error status
    // required because the pi has a known serial glitch that causes the servo controller to enter error mode
    serialPutchar(fd, 0xA1);
    serialPutchar(fd, 0xA1);
}

PiPololuMotorControl::~PiPololuMotorControl() {
    serialClose(fd);
}

void PiPololuMotorControl::writeServo(int servo, float angle) {
    
    // map [0, 1] to [servoMin, servoMax]
    unsigned short servoPosition = (short) 
            (angle * (SERVO_MAX - SERVO_MIN) + SERVO_MIN);
    
    serialPutchar(fd, 0x84);
    serialPutchar(fd, servo);
    serialPutchar(fd, servoPosition & 0x7F);
    serialPutchar(fd, (servoPosition >> 7) & 0x7F);
}

void PiPololuMotorControl::setMotors(float joints[], int numJoints) {
    
    int servo = 0;
    for (unsigned char i = 0; i < numJoints; i++) {
        writeServo(servo, joints[servo++]); // coxa
        writeServo(servo, joints[servo++]); // femur
        writeServo(servo, joints[servo++]); // tibia
    }
}
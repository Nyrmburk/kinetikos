/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "PiPololuMotorControl.h"

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

void writec(const int fd, const uint8_t c) {
    write(fd, &c, 1);
}

PiPololuMotorControl::PiPololuMotorControl(int fd) {
    this->fd = fd;
    
    // clears the error status
    // required because the pi has a known serial glitch that causes the servo controller to enter error mode

    writec(fd, 0xA1);
    writec(fd, 0xA1);
}

PiPololuMotorControl::~PiPololuMotorControl() {
    close(fd);
}

void PiPololuMotorControl::writeServo(int servo, float angle) {
    // map [0, 1] to [servoMin, servoMax]
    unsigned short servoPosition = (short) 
            (angle * (SERVO_MAX - SERVO_MIN) + SERVO_MIN);
    
    writec(fd, 0x84);
    writec(fd, servo);
    writec(fd, servoPosition & 0x7F);
    writec(fd, (servoPosition >> 7) & 0x7F);
}

void PiPololuMotorControl::setMotors(Joints* joints, int numJoints) {
    int servo = 0;
    for (unsigned char i = 0; i < numJoints; i++) {
        writeServo(servo++, joints[i].coxa);
        writeServo(servo++, joints[i].femur);
        writeServo(servo++, joints[i].tibia);
    }
}


int PiPololuMotorControl::getSerial() {
    int fd = open(DEVICE, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
    if (fd == -1) {
        return fd;
    }

    fcntl(fd, F_SETFL, O_RDWR);

    struct termios options;
    tcgetattr(fd, &options);
    cfmakeraw(&options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;

    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 100;

    tcsetattr(fd, TCSANOW | TCSAFLUSH, &options);

    int status;
    ioctl(fd, TIOCMGET, &status);

    status |= TIOCM_DTR;
    status |= TIOCM_RTS;

    ioctl(fd, TIOCMSET, &status);
    
    usleep(10000);

    return fd;
}

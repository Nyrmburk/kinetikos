/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   h264.h
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 2:25 PM
 */

#ifndef H264_H
#define H264_H

#include <cstdio>

class h264Provider {
public:
    h264Provider(FILE source);
    virtual ~h264Provider();
    void run();
private:
    FILE* source;
    unsigned char* frame;
    size_t size;
    size_t index;
};

#endif /* H264_H */


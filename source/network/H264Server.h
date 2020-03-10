/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Server.h
 * Author: nyrmburk
 *
 * Created on May 12, 2018, 9:39 PM
 */

#ifndef H264SERVER_H
#define H264SERVER_H

#include <vector>
#include <uWS/uWS.h>

#define BLOCK_SIZE 4096

class H264Server {
public:
    H264Server();
    virtual ~H264Server();
    
    void run();
private:
uWS::Hub hub;
FILE* camera;
std::vector<char> buffer;
int index = 0;
uint32_t history = 0xFFFFFFFF;
};

#endif /* H264SERVER_H */


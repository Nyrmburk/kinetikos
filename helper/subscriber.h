/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   subscriber.h
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 2:23 PM
 */

#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include "publisher.h"

class publisher; // forward reference

class subscriber {
public:
    virtual void onEvent(const void* event) = 0;
    virtual ~subscriber() {};
};

#endif /* SUBSCRIBER_H */


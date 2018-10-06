/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   publisher.h
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 2:23 PM
 */

#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <list>
#include "subscriber.h"

using namespace std;

class publisher {
public:
    void notify(const void* event);
    void subscribe(subscriber* s);
    void unsubscribe(subscriber* s);
    virtual ~publisher() {};
private:
    list<subscriber*> subscribers;
};

#endif /* PUBLISHER_H */


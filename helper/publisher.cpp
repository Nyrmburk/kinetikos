/*
 * File:   publisher.cpp
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 2:23 PM
 */

#include "publisher.h"

void publisher::notify(const void* event) {
    list<subscriber*>::iterator iter;
    for (iter = subscribers.begin(); iter != subscribers.end(); iter++) {
        (*iter)->onEvent(event);
    }
}

void publisher::subscribe(subscriber* s) {
    subscribers.push_back(s);
}

void publisher::unsubscribe(subscriber* s) {
    subscribers.remove(s);
}

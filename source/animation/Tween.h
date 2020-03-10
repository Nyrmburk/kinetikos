/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tween.h
 * Author: nyrmburk
 *
 * Created on June 24, 2017, 10:26 AM
 */

#ifndef TWEEN_H
#define TWEEN_H

typedef float (*easeFunc)(float);

float easeNone(float k);
float easeLinear(float k);
float easeQuadraticIn(float k);
float easeQuadraticOut(float k);
float easeQuadraticInOut(float k);

template <typename T>
class Tween {
public:
    Tween() {}
    Tween(const T& value, float time, easeFunc function) {
        setValue(value);
        this->time = time;
        setFunction(function);
    }
    
    float ease(float k) {
        return function(k);
    }
    
    T* getValue() {
        return &value;
    }
    
    void setValue(const T& t)  {
        value = t;
    }
    
    float getTime() const {
        return time;
    }
    
    easeFunc getFunction() {
        return function;
    }
    
    void setFunction(easeFunc function) {
        this->function = function;
    }
private:
    T value;
    float time;
    easeFunc function;
};

#endif /* TWEEN_H */

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RemoteControlIntent.h
 * Author: nyrmburk
 *
 * Created on May 8, 2018, 9:48 PM
 */

#ifndef REMOTECONTROLINTENT_H
#define REMOTECONTROLINTENT_H

class RemoteControlIntent : public Intent {
public:
    void start() {
        
    }
    
    void step(float delta) {
        // get remote control data from remote control system
        
        // set velocity in velocity step
        // that should kick things off right?
        // or do I have to manually follow things down the chain?
        // ughh decisions decisions
        // the way I had always thought of it was moving closer down the stack
        // so I guess the best way is to make each controller feed into the next layer
        // the difficulty is getting the differences between animation and simulation loops
        // I think I have that figured out now though
    }
    
    void stop() {
        
    }

#endif /* REMOTECONTROLINTENT_H */


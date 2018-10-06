/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tween_channel.h
 * Author: nyrmburk
 *
 * Created on June 24, 2017, 1:15 PM
 */

#ifndef TWEEN_CHANNEL_H
#define TWEEN_CHANNEL_H

#include <vector>
#include "Tween.h"

using namespace std;

// this provides the interface for directly manipulating the value in question
// it has to call a set value function at every step
// it has to keep track of the timeline time value
// maybe I need to take some functionality from tween and put it here?
// it's getting pretty muddy with having lists here of having tween be a list
// I don't know what I am going to do.
// I just need to look at the timeline and start delegating
// it's so hard to be smart
// why is it so hard to be smart
// why does everything take so long
// I just want to see the damn robot dancing
// maybe I need a break

// tween has a time, function, value
// a channel has a sorted list of tweens, has a "master" value that is referenced after every step
// sequencer has a list of channels, animation length, current time,

// the sequencer for animation and gait control is very similar
// in fact, I was planning on using the very same method for controlling the feet
// hmmmm
// I did say that the feet were going to be manipulated either directly or by the animator
// it looks like the main difference is what the "current time" value represents
// in gait control the value speeds up when traveling faster
// in animation it is directly tied to the actual time
// perhaps I could get away with a time scale or directly manipulating the tweens
// timescale is cleaner and adds extra functionality for animation playback

// something to figure out with the sequencer is what to do when a tween is done
// save it for a loop or consume it?
// I can say that the remote editor will be directly setting feet positions
// this means that the default state could be consumption
// consumption makes it easier for on the fly animation
// I think I planned for consumption anyway
// that way it would always be the first and second tween

template <typename T>
class TweenChannel {
public:
    TweenChannel(T& value) : value(value) {};
//    virtual ~TweenChannel() {};
    
    void getTweens(float t, Tween<T>* start, Tween<T>* end) {
        
        for (unsigned int i = 0; i < tweens.size(); i++) {
            if (tweens[i].getTime() > t) {
                *end = tweens[i];
                break;
            } else {
                *start = tweens[i];
            }
        }

        if (!end)
            *end = *start;
    }
    
    void insertTween(Tween<T>& t) {
        long i = tweens.size();
        while (i --> 0) {
            if (tweens[i].getTime() < t.getTime())
                break;
        }
        tweens.insert(tweens.begin() + i + 1, t);
    }
    
    void removeTween(Tween<T>& t) {
        tweens.erase(t);
    }
    
    void step(float t) {
        Tween<T> start, end;
        getTweens(t, &start, &end);
        float k = start.ease(t);
        ease(start.getValue(), end.getValue(), k, &value);
    }
    
    virtual void ease(T* start, T* end, float k, T* result) = 0;
private:
    T& value;
    vector<Tween<T>> tweens;
};

#endif /* TWEEN_CHANNEL_H */

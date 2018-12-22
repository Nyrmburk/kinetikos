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

template <typename I, typename O>
class TweenChannel {
public:
    bool getTweens(float time, Tween<I>* returnStart, Tween<I>* returnEnd) const {
        if (tweens.size() == 0) {
            return false;
        }

        for (size_t i = 0; i < tweens.size(); i++) {
            if (time < tweens[i].getTime()) {
                *returnEnd = tweens[i];
                return true;
            } else {
                *returnStart = tweens[i];
            }
        }

        // there's only one element left in the list
        *returnEnd = *returnStart;
        return true;
    }
    
    void insertTween(const Tween<I>& t) {
        size_t i = tweens.size();
        while (i --> 0) { // start looking backwards from the end
            if (tweens[i].getTime() <= t.getTime())
                break;
        }
        tweens.insert(tweens.begin() + i + 1, t);
    }
    
    void removeTween(const Tween<I>& t) {
        tweens.erase(t);
    }

    void clear() {
        tweens.clear();
    }
    
    void act(float time, O& result) const {
        Tween<I> start, end;
        if (getTweens(time, &start, &end)) {
            float range = end.getTime() - start.getTime();
            float t = 0;
            if (range != 0.0f) {
                t = (time - start.getTime()) / range;
            }
            float k = start.ease(t);
            ease(start.getValue(), end.getValue(), k, &result);
        }
    }
    
    virtual void ease(I* start, I* end, float k, O* result) const = 0;
private:
    vector<Tween<I>> tweens;
};

#endif /* TWEEN_CHANNEL_H */

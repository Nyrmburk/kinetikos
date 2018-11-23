#ifndef ROBOT_CLIP_H
#define ROBOT_CLIP_H

#include <vector>
#include "../matrix/mat4.h"
#include "../matrix/vec3.h"
#include "AnimationClip.h"
#include "Mat4Channel.h"
#include "Bezier3Channel.h"

class RobotClip : public AnimationClip {
public:
    RobotClip(float length,
            Mat4Channel bodyChannel, size_t footCount, Bezier3Channel* footChannels) :
            AnimationClip(length),
            bodyChannel(bodyChannel),
            footCount(footCount),
            footChannels(footChannels) {}

    void setTargets(Mat4* bodyOrientation, Vec3* feet) {
        this->bodyOrientation = bodyOrientation;
        this->feet = feet;
    }

    void act(float time) {
        bodyChannel.act(time, *bodyOrientation); 

        for (size_t i = 0; i < footCount; i++) {
            footChannels[i].act(time, feet[i]);
        }
    }
private:
    Mat4* bodyOrientation = nullptr;
    Vec3* feet = nullptr;

    Mat4Channel bodyChannel;
    size_t footCount;
    Bezier3Channel* footChannels;
};

#endif /* ROBOT_CLIP_H */

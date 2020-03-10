#ifndef JOYSTICK_PLAN_H
#define JOYSTICK_PLAN_H

#include "MotionPlan.h"
#include "../matrix/transform.h"
#include "../matrix/vec2.h"
#include "../matrix/vec3.h"

class JoystickPlan : public MotionPlan {
public:
    JoystickPlan() {}

    JoystickPlan(float maxVelocity, float maxAcceleration,
                float maxRotationVelocity, float maxRotationAcceleration) {
        this->maxVelocity = maxVelocity;
        this->maxAcceleration = maxAcceleration;
        this->maxRotationVelocity = maxRotationVelocity;
        this->maxRotationAcceleration = maxRotationAcceleration;

        identitym4(&this->orientation);
    }

    struct JoystickPair {
        Vec2 xy = {};
        Vec2 uv = {};

        void mix(JoystickPair& joysticks) {
            addv2(&xy, &joysticks.xy, &xy);
            addv2(&uv, &joysticks.uv, &uv);
        }

        void normalize() {
            bound(&xy, &xy);
            bound(&uv, &uv);
        }

        void clear() {
            xy.x = 0;
            xy.y = 0;
            uv.x = 0;
            uv.y = 0;
        }
    };
    
    void orientationAt(float delta, Mat4* orientation) {
        Vec2 xyFuture;
        multiplyv2s(&joysticks.xy, maxVelocity * delta, &xyFuture);

        Vec2 uvFuture;
        multiplyv2s(&joysticks.uv, maxRotationVelocity * delta, &uvFuture);

        Vec3 translation;
        v2asv3(&xyFuture, 0, &translation);

        Mat4 orientationDelta;
        identitym4(&orientationDelta);

        // convert uvFuture to cartesian
        // then use lookat to look at the point
        rotate(&orientationDelta, &Z_AXIS, -uvFuture.x);

        translate(&orientationDelta, &translation);

        multm4m4(&this->orientation, &orientationDelta, orientation);
    }

    void step(float delta) {
        MotionPlan::step(delta);

        joysticks.clear();
        for (auto it = channels.begin(); it < channels.end(); ++it) {
            joysticks.mix(**it);
        }
        joysticks.normalize();

        Mat4 temp;
        orientationAt(delta, &temp);
        setm4(&orientation, &temp);
    }

    void addChannel(JoystickPair* joysticks) {
        channels.push_back(joysticks);
    }

    void removeChannel(JoystickPair* joysticks) {
        channels.erase(std::remove(channels.begin(), channels.end(), joysticks), channels.end());
    }
private:
    Mat4 orientation = {};
    JoystickPair joysticks = {};

    vector<JoystickPair*> channels;

    float maxVelocity;
    float maxAcceleration;
    float maxRotationVelocity;
    float maxRotationAcceleration;

    static void bound(Vec2* input, Vec2* result) {
        float length = lengthv2(input);
        if (length > 1) {
            dividev2s(input, length, result);
        } else {
            setv2(result, input);
        }
    }
};

#endif /* JOYSTICK_PLAN_H */

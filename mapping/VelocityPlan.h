#ifndef VELOCITY_PLAN_H
#define VELOCITY_PLAN_H

#include "MotionPlan.h"
#include "../matrix/transform.h"
#include "../matrix/vec2.h"
#include "../matrix/vec3.h"

class VelocityPlan : public MotionPlan {
public:
    VelocityPlan() {}

    VelocityPlan(Vec2* position, float rotation) {
        setv2(&this->position, position);
        this->rotation = rotation;
    }
    
    void orientationAt(float delta, Mat4* orientation) {
        identitym4(orientation);

        rotate(orientation, &Z_AXIS, rotation + (rotationVelocity * delta));

        Vec2 newPosition;
        multiplyv2s(&velocity, delta, &newPosition);
        addv2(&newPosition, &position, &newPosition);
        Vec3 position3;
        v2asv3(&newPosition, 0, &position3);
        setPosition(orientation, &position3);
    }

    void step(float delta) {
        MotionPlan::step(delta);
        Vec2 tempVelocity;
        multiplyv2s(&velocity, delta, &tempVelocity);
        addv2(&position, &tempVelocity, &position);
        rotation += rotationVelocity * delta;
    }

    void set(Vec2 velocity, float rotationVelocity) {
        setv2(&this->velocity, &velocity);
        this->rotationVelocity = rotationVelocity;
    }
private:
    Vec2 position = {0, 0};
    Vec2 velocity = {0, 0};
    float rotation = 0;
    float rotationVelocity = 0;
};

#endif /* VELOCITY_PLAN_H */

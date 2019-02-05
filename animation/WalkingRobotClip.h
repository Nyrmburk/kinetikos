#ifndef WALKING_ROBOT_CLIP_H
#define WALKING_ROBOT_CLIP_H

#include <vector>
#include <map>
#include <limits>
#include "../robot/Gait.h"
#include "../matrix/mat4.h"
#include "../matrix/vec3.h"
#include "AnimationClip.h"
#include "Mat4Channel.h"
#include "Bezier3Channel.h"

class WalkingRobotClip : public AnimationClip {
public:
    WalkingRobotClip(Robot* robot, Gait* gait) : AnimationClip() {
        this->length = std::numeric_limits<float>::infinity();

        this->robot = robot;
        this->gait = gait;
        
        bodyChannel = new Mat4Channel;
        Mat4 removeMe;
        identitym4(&removeMe);
        removeMe.m[14] = 40;
        Tween<Mat4> heck(removeMe, 0, easeNone);
        bodyChannel->insertTween(heck);
        
        footCount = robot->getBody()->legsCount;
        footChannels = new Bezier3Channel[footCount];

        steps = new deque<StepPlan>[footCount];
        stepsNow = new StepPlan[footCount];
        Mat4* orientation = robot->getOrientation();
        Mat4 invOrientation;
        inversem4(orientation, &invOrientation);
        for (int i = 0; i < footCount; i++) {
            stepsNow[i].land(getTime(), orientation, &invOrientation);
            stepsNow[i].lift(getTime(), orientation, &invOrientation);
            multm4v3(orientation, &robot->getFeet()[i], 1, &stepsNow[i].wsPosition);
        }
    }

    virtual ~WalkingRobotClip() {
        // TODO: outdated
        delete bodyChannel;
        delete[] footChannels;
    }

    void setTargets(Mat4* bodyOrientation, Vec3* feet) {
        this->bodyOrientation = bodyOrientation;
        this->feet = feet;
    }

    void sim(float delta) {
        float time = getTime();
        // feet from past sim and future sim to calculate gait interval from greatest foot speed
        // ws means worldspace
        Vec3 wsFeetThen[footCount];
        Vec3 wsFeetFuture[footCount];

        // past and future orientations
        Mat4 orientationNow;
        Mat4 invOrientationNow;
        Mat4 orientationFuture;
        Mat4 invOrientationFuture;

        // get the current orientation
        robot->getMotionPlan()->orientationAt(time, &orientationNow);
        // inverse used to convert worldspace to robotspace
        inversem4(&orientationNow, &invOrientationNow);

        // get foot position at time now
        for (int i = 0; i < footCount; i++) {
            multm4v3(&orientationNow, &robot->getFeetHome()[i], 1, &wsFeetFuture[i]);
            //setv3(&wsFeetFuture[i], &stepsNow[i].wsPosition);
        }

        // clearing invalid predictions
        float voidTime = time - delta;
        clearFutureVoidedPlans(voidTime);

        for (int i = 0; i < footCount; i++) {
            if (steps[i].empty()) {
                steps[i].push_back(stepsNow[i]);
            }
        }

        float future = 0; // delta between now and future sim
        float cursor = gaitCursor; // future sim cursor position
        bool wasGroundedNow[footCount];
        bool wasGrounded[footCount];
        for (int i = 0; i < footCount; i++) {
            wasGroundedNow[i] = isGrounded(i, steps[i].front(), gaitCursor);
            //wasGrounded[i] = gait[i].isGrounded(gaitCursor);
            wasGrounded[i] = wasGroundedNow[i];
        }

        float lastFuture = 0;
        do {
            lastFuture = future;
            future += delta;

            // calculate future orientation
            robot->getMotionPlan()->orientationAt(future, &orientationFuture);
            inversem4(&orientationFuture, &invOrientationFuture);
            for (int i = 0; i < footCount; i++) {
                setv3(&wsFeetThen[i], &wsFeetFuture[i]); // update the previous foot positions
                multm4v3(&orientationFuture, &robot->getFeetHome()[i], 1, &wsFeetFuture[i]);
            }

            // increment gait cursor
            cursor += delta;

            // move the foot and check if the new foot plan is out of the land/lift workspaces
            Vec3 diff;
            Mat4 bodyOrientationFuture;
            bodyChannel->act(time + future, bodyOrientationFuture);
            int abort = 0;
            StepPlan oldSteps[footCount];
            bool oldGrounded[footCount];
            for (int i = 0; i < footCount; i++) {
                oldSteps[i] = steps[i].back();
                oldGrounded[i] = wasGrounded[i];
            }
            for (int i = 0; i < footCount; i++) {
                StepPlan& stepFuture = steps[i].back();

                if (wasGrounded[i]) {
                    stepFuture.lift(time + future, &orientationFuture, &invOrientationFuture);
                    if (steps[i].size() > 1) {
                        subtractv3(&wsFeetFuture[i], &wsFeetThen[i], &diff);
                        dividev3s(&diff, 2, &diff);
                        Vec3& wsPos = stepFuture.wsPosition;
                        addv3(&wsPos, &diff, &wsPos);

                        float footSeparation = 50;
                        for (int j = 0; j < footCount; j++) {
                            if (i == j || lengthv3(&diff) < 0.01) {
                                continue;
                            }

                            Vec3 footDiff;
                            subtractv3(&steps[j].back().wsPosition, &wsPos, &footDiff);
                            float distance = lengthv3(&footDiff);
                            float angle = fabs(anglev3(&footDiff, &diff));
                            angle = min(angle, fabs(anglev3(&diff, &footDiff)));
                            if (distance < footSeparation) {
                                if (angle < M_PI_2) {
                                    abort = 1;
                                    break;
                                } else {
                                    multiplyv3s(&footDiff, -1 / distance, &footDiff);
                                    addv3(&wsPos, &footDiff, &wsPos);
                                }
                            }
                        }
                        if (abort) {
                            break;
                        }
                    }

                    if (distanceFromHome(stepFuture, &robot->getFeetHome()[i]) > 50) {
                        abort = 2;
                        break;
                    }

                    Leg& leg = robot->getBody()->legs[i];
                    Vec3 rsFoot;

                    // check if the current plan is outside of the workspace at lift-time
                    bool liftCollision = false;
                    stepFuture.getRsLiftPos(&rsFoot);
                    if (!isFootInWorkspace(&bodyOrientationFuture, &rsFoot, leg)) {
                        liftCollision = true;
                        if (steps[i].size() > 1) {
                            // move the plan back into the workspace
                            addv3(&stepFuture.wsPosition, &diff, &stepFuture.wsPosition);
                            // TODO snap the foot to the world position
                        } else {
                            abort = 3;
                            break;
                        }
                    }

                    // check if the current plan is outside of the workspace at land-time
                    bool landCollision = false;
                    stepFuture.getRsLandPos(&rsFoot);
                    if (!isFootInWorkspace(&bodyOrientationFuture, &rsFoot, leg)) {
                        landCollision = true;
                        if (steps[i].size() > 1) {
                            // move the plan back into the workspace
                            subtractv3(&stepFuture.wsPosition, &diff, &stepFuture.wsPosition);
                            // TODO snap the foot to the world position
                        } else {
                            abort = 4;
                            break;
                        }
                    }

                    if (liftCollision && landCollision) {
                        abort = 5;
                        break;
                    }
                }
            }

            if (abort) {
                for (int i = 0; i < footCount; i++) {
                    steps[i].back() = oldSteps[i];

                    //TODO possibly reinstate this when the original bug is found.
                    //setv3(&wsFeetFuture[i], &wsFeetThen[i]); // reset future to previous
                    wasGrounded[i] = oldGrounded[i];
                }
                future = lastFuture;
            }

            // check if foot landed?
            for (int i = 0; i < footCount; i++) {
                bool grounded = gait[i].isGrounded(cursor);
                if (wasGrounded[i] < grounded) {
                    // landed
                    steps[i].emplace_back();
                    steps[i].back().land(time + future, &orientationFuture, &invOrientationFuture);
                    multm4v3(&orientationFuture, &robot->getFeetHome()[i], 1, &steps[i].back().wsPosition);
                    cursorTimes[time + future] = cursor;
                } else if (wasGrounded[i] > grounded) {
                    // lifted
                    cursorTimes[time + future] = cursor;
                }
                wasGrounded[i] = grounded;
            }
        } while (cursor < gaitCursor + 1);
        // after looping

        float simTimeFuture = time + delta;
        robot->getMotionPlan()->orientationAt(delta, &orientationFuture);
        inversem4(&orientationFuture, &invOrientationFuture);

        // update animation tweens
        for (int i = 0; i < footCount; i++) {
            footChannels[i].clear();
            StepPlan& step = steps[i].front();
            StepPlan& stepNext = steps[i][1];
            if (isGrounded(i, step, gaitCursor)) {
                // current step is now grounded
                // current land -> current lift
                Vec3 handle = {0, 0, 0};

                // current step land
                Bezier3::Node startNode;
                multm4v3(&invOrientationNow, &step.wsPosition, 1, &startNode.point);
                setv3(&startNode.handle, &handle);
                Tween<Bezier3::Node> start(startNode, time, easeLinear);
                footChannels[i].insertTween(start);
                
                // current step lift
                Bezier3::Node endNode;
                multm4v3(&invOrientationFuture, &step.wsPosition, 1, &endNode.point);
                setv3(&endNode.handle, &handle);
                Tween<Bezier3::Node> end(endNode, simTimeFuture, easeNone);
                footChannels[i].insertTween(end);
            } else {
                // current step is now lifted
                // current lift -> next land
                Vec3 handle = {0, 0, 50};

                // current step lift
                Bezier3::Node startNode;
                step.getRsLiftPos(&startNode.point);
                setv3(&startNode.handle, &handle);
                Tween<Bezier3::Node> start(startNode, step.liftTime, easeQuadraticInOut);
                footChannels[i].insertTween(start);

                // next step land
                Bezier3::Node endNode;
                stepNext.getRsLandPos(&endNode.point);
                setv3(&endNode.handle, &handle);
                Tween<Bezier3::Node> end(endNode, stepNext.landTime, easeNone);
                footChannels[i].insertTween(end);
            }
        }

        gaitCursor = getCursor(gaitCursor, delta, time, 0);

        // clearing old plans
        clearUsedPlans(time);

        for (int i = 0; i < footCount; i++) {
            bool grounded = isGrounded(i, steps[i].front(), gaitCursor);
            if (wasGroundedNow[i] < grounded) {
                // landed
                steps[i].pop_front();
                stepsNow[i] = steps[i].front();
            } else if (wasGroundedNow[i] > grounded) {
                // lifted
                stepsNow[i].lift(simTimeFuture, &orientationFuture, &invOrientationFuture);
            }
        }
    }

protected:
    void act(float time) {
        bodyChannel->act(time, *bodyOrientation); 

        for (size_t i = 0; i < footCount; i++) {
            footChannels[i].act(time, feet[i]);
        }
    }

private:
    Robot* robot;

    Mat4* bodyOrientation = nullptr;
    Vec3* feet = nullptr;

    Mat4Channel* bodyChannel;
    size_t footCount;
    Bezier3Channel* footChannels;

    Gait* gait;
    float gaitCursor = 0;
    
    struct StepPlan {
        float landTime = 0;
        float liftTime = 0;

        Mat4 orientationLand;
        Mat4 invOrientationLand;
        Mat4 orientationLift;
        Mat4 invOrientationLift;

        Vec3 wsPosition;

        bool isPositionMovable(float timeNow) {
            return timeNow <= landTime;
        }

        void getRsLandPos(Vec3* pos) {
            multm4v3(&invOrientationLand, &wsPosition, 1, pos);
        }

        void getRsLiftPos(Vec3* pos) {
            multm4v3(&invOrientationLift, &wsPosition, 1, pos);
        }

        void land(float time, Mat4* orientation, Mat4* invOrientation) {
            landTime = time;
            setm4(&orientationLand, orientation);
            setm4(&invOrientationLand, invOrientation);
        }

        void lift(float time, Mat4* orientation, Mat4* invOrientation) {
            liftTime = time;
            setm4(&orientationLift, orientation);
            setm4(&invOrientationLift, invOrientation);
        }

        StepPlan& operator= (const StepPlan& plan) {
            if (this != &plan) {
                landTime = plan.landTime;
                liftTime = plan.liftTime;

                setm4(&orientationLand, &plan.orientationLand);
                setm4(&invOrientationLand, &plan.invOrientationLand);
                setm4(&orientationLift, &plan.orientationLift);
                setm4(&invOrientationLift, &plan.invOrientationLift);

                setv3(&wsPosition, &plan.wsPosition);
            }
            return *this;
        }
    };
    deque<StepPlan>* steps;
    StepPlan* stepsNow;

    map<float, float> cursorTimes;

    bool isFootInWorkspace(Mat4* bodyOrientation, Vec3* rsFoot, Leg& leg) {
        Vec3 actualFoot;
        Joints joints;
        return leg.solveInverse(bodyOrientation, rsFoot, &joints, &actualFoot);
    }

    float getCursor(float cursor, float delta, float time, float maxSpeed) {
        float speed = delta; // should be calculated

        auto cursorTime = cursorTimes.upper_bound(time);
        if (cursorTime == cursorTimes.end()) {
            // no future cursor time, so falling back
            cursor += speed;
        } else {
            cursor = mapRange(
                    delta, 0, cursorTime->first - time,
                    gaitCursor, cursorTime->second);
        }

        return cursor;
    }

    float distanceFromHome(StepPlan& step, Vec3* home) {
        Vec3 rsPos;
        step.getRsLiftPos(&rsPos);
        float distance = distancev3(&rsPos, home);
        step.getRsLandPos(&rsPos);
        distance = max(distance, distancev3(&rsPos, home));
        return distance;
    }

    bool isGrounded(int i, StepPlan& step, float cursor) {
        float distance = distanceFromHome(step, &robot->getFeetHome()[i]);
        return gait[i].isGrounded(cursor) || distance < 10;
    }

    void clearFutureVoidedPlans(float voidTime) {

        for (int i = 0; i < footCount; i++) {
            for (auto it = steps[i].begin(); it != steps[i].end(); ++it) {
                if (voidTime <= (*it).landTime) {
                    cursorTimes.erase(cursorTimes.upper_bound((*it).liftTime),
                            cursorTimes.end());
                    steps[i].erase(it, steps[i].end());
                    break;
                }
            }
        }
    }

    void clearUsedPlans(float timeNow) {
        for (int i = 0; i < footCount; i++) {
            for (auto it = steps[i].begin(); it != steps[i].end(); ++it) {
                if ((*it).liftTime <= timeNow) {
                    cursorTimes.erase(cursorTimes.begin(),
                            cursorTimes.lower_bound((*it).landTime));
                    steps[i].erase(steps[i].begin(), it);
                    break;
                }
            }
        }
    }

    float getFootSpeed(Vec3* feetStart, Vec3* feetEnd, float delta) {
        float footSpeed = 0;
        for (int i = 0; i < footCount; i++) {
            float distance = distancev3(&feetStart[i], &feetEnd[i]);
            footSpeed = std::max(footSpeed, distance);
        }
        footSpeed /= delta;
        return footSpeed;
    }
};

#endif /* WALKING_ROBOT_CLIP_H */

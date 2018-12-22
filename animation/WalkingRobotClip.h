#ifndef WALKING_ROBOT_CLIP_H
#define WALKING_ROBOT_CLIP_H

#include <vector>
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
        removeMe.m[14] = 20;
        Tween<Mat4> heck(removeMe, 0, easeNone);
        bodyChannel->insertTween(heck);
        
        footCount = robot->getBody()->legsCount;
        footChannels = new Bezier3Channel[footCount];

        steps = new deque<StepFrame>[footCount];
        workingSteps = new StepFrame*[footCount];
        for (uint8_t i = 0; i < footCount; i++) {
            workingSteps[i] = new StepFrame;
        }   
    }

    virtual ~WalkingRobotClip() {
        delete bodyChannel;
        delete[] footChannels;
    }

    void setTargets(Mat4* bodyOrientation, Vec3* feet) {
        this->bodyOrientation = bodyOrientation;
        this->feet = feet;
    }

    void sim(float time, float delta) {
        // calculate per-foot speed

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
        setm4(&orientationNow, robot->getOrientation());
        cout << "now: " << orientationNow.m[12] << ", " << orientationNow.m[13] << endl;
        // inverse used to convert robotspace to worldspace
        inversem4(&orientationNow, &invOrientationNow);

        // get foot position at time now
        cout << "updating the foot home position" << endl;
        for (uint8_t i = 0; i < footCount; i++) {
            multm4v3(&orientationNow, &robot->getFeetHome()[i], 1, &wsFeetFuture[i]);
        }

        // clearing invalid predictions
        float dirtyTime = simTime;
        for (uint8_t i = 0; i < footCount; i++) {
            while (!steps[i].empty() && dirtyTime < steps[i].back().simTime) {
                steps[i].pop_back();
            }
            if (steps[i].empty()) {
                steps[i].push_front(*workingSteps[i]);
            }
        }   

        float future = 0; // delta between now and future sim
        float cursor = gaitCursor; // future sim cursor position

        cout << endl << endl << "predicting future..." << endl;
        do {
            //cout << endl << "start iteration: " << cursor << endl;
            future += delta;

            // get foot position in the future
//            setm4(&orientationThen, &orientationFuture); // update the previous orientation
            robot->getMotionPlan()->orientationAt(future, &orientationFuture); // calculate future orientation
            for (uint8_t i = 0; i < footCount; i++) {
                setv3(&wsFeetThen[i], &wsFeetFuture[i]); // update the previous foot positions
                multm4v3(&orientationFuture, &robot->getFeetHome()[i], 1, &wsFeetFuture[i]);
            }

            // compare foot positions to get distance traveled
            float footSpeed = 0;
            for (uint8_t i = 0; i < footCount; i++) {
                float distance = distancev3(&wsFeetThen[i], &wsFeetFuture[i]);
                footSpeed = std::max(footSpeed, distance);
            }
            //cout << "foot speed: " << footSpeed << endl;
            footSpeed /= delta;

            // calculate step distance
            float stepDistance = footSpeed;

            // calculate gait interval
            float interval = stepDistance / footSpeed;
            interval += interval; // multiply by 2
            interval = 1; // TODO: remove once interval/speed is cleaned up

            // increment gait cursor
            // see if I can simplify most of the math behind this
            cursor += interval * delta;

            // calculate average grounded orientation and time of lift and land
            //cout << "current cursor: " << cursor << endl;
            for (uint8_t i = 0; i < footCount; i++) {
                StepFrame& frame = steps[i].back();
                bool tookStep = simGait(simTime + future, &orientationFuture, frame, gait[i], cursor);
                if (tookStep) {
                    //cout << "new step at: " << simTime + future << endl;
                    // the step is complete, create a new working step
                    steps[i].emplace_back();
                }
            }

            //cout << "future: " << future << endl;
        } while (cursor < gaitCursor + 1);
        // after looping

        gaitCursor += delta; // delta * interval
        cout << "new cursor: " << gaitCursor << endl;

        // calculate leg targets
        robot->getMotionPlan()->orientationAt(delta, &orientationFuture);
        inversem4(&orientationFuture, &invOrientationFuture);
        Vec3 wsTargets[footCount];
        Vec3 rsTargets[footCount];
        for (uint8_t i = 0; i < footCount; i++) {
            while (!steps[i].empty() && steps[i].front().simTime < simTime) {
                steps[i].pop_front(); // clear old steps
            }
            StepFrame& step = steps[i].front();
            Mat4* ago = &step.accumulatedGroundedOrientation;
            cout << "ago: it: " << step.groundedIterations << ", x: " << ago->m[12] << ", y: " << ago->m[13] << endl;
            Mat4 avgGroundedOrientation;
            multm4s(ago, (1.0f / step.groundedIterations), &avgGroundedOrientation);
            cout << "x: " << avgGroundedOrientation.m[12] << ", y: " << avgGroundedOrientation.m[13] << endl;
            cout << (int) i << ": land:" << step.landTime << ", lift:" << step.liftTime << endl;
            cout << (int) steps[i].size() << " steps planned ahead" << endl;

            multm4v3(&avgGroundedOrientation, &robot->getFeetHome()[i], 1, &wsTargets[i]);
            multm4v3(&invOrientationFuture, &wsTargets[i], 1, &rsTargets[i]);
            cout << "target: " << rsTargets[i].x << ", " << rsTargets[i].y << endl;
        }

        // find new rs grounded foot position
        Vec3 wsFeetNow[footCount];
        Vec3 rsFeetFuture[footCount];
        for (uint8_t i = 0; i < footCount; i++) {
            multm4v3(&orientationNow, &robot->getFeet()[i], 1, &wsFeetNow[i]);
            multm4v3(&invOrientationFuture, &wsFeetNow[i], 1, &rsFeetFuture[i]);
            cout << "rs grounded: x: " << rsFeetFuture[i].x << ", y: " << rsFeetFuture[i].y << endl;
        }

        float simTimeFuture = simTime + delta;
        // update tweens
        for (uint8_t i = 0; i < footCount; i++) {
            footChannels[i].clear();
            if (gait[i].isGrounded(gaitCursor)) {
                cout << "foot " << (int) i << " grounded" << endl;
                Vec3 handle = {0, 0, 0};

                Bezier3::Node startNode;
                setv3(&startNode.point, &robot->getFeet()[i]);
                setv3(&startNode.handle, &handle);
                Tween<Bezier3::Node> start(startNode, simTime, easeLinear);
                footChannels[i].insertTween(start);
                cout << "start: " << startNode.point.x << ", " << startNode.point.y << endl;
                
                Bezier3::Node endNode;
                setv3(&endNode.point, &rsFeetFuture[i]);
                setv3(&endNode.handle, &handle);
                Tween<Bezier3::Node> end(endNode, simTimeFuture, easeNone);
                footChannels[i].insertTween(end);
                cout << "end: " << endNode.point.x << ", " << endNode.point.y << endl;
            } else {
                Vec3 handle = {0, 0, 20};
                cout << "foot " << (int) i << " lifted" << endl;

/*
                Bezier3::Node startNode;
                setv3(&startNode.point, &robot->getFeetHome()[i]);
                setv3(&startNode.handle, &handle);
                Tween<Bezier3::Node> start(startNode, simTime, easeLinear);
                footChannels[i].insertTween(start);
                cout << "start: " << startNode.point.x << ", " << startNode.point.y << endl;
                */

                Bezier3::Node endNode;
                setv3(&endNode.point, &rsTargets[i]);
                setv3(&endNode.handle, &handle);
                Tween<Bezier3::Node> end(endNode, simTime, easeNone);
                footChannels[i].insertTween(end);
                Tween<Bezier3::Node> end2(endNode, simTimeFuture, easeNone);
                footChannels[i].insertTween(end2);
                cout << "end: " << endNode.point.x << ", " << endNode.point.y << endl;

                // ??? = what?
                // original land position?
                // current position?
                // it's really difficult to make the new tween match if I change it
                //start = (landTime[i], ???);
                //end = (liftTime[i], rsTargets[i])
    //            cout << "start: " << landTime[i] << ", " << "???" << endl;
    //            cout << "end:   " << liftTime[i] << ", (" <<
    //                    rsTargets[i].x << ", " << rsTargets[i].y << ", " << rsTargets[i].z << endl;
            }
        }

        simTime = simTimeFuture;
        for (uint8_t i = 0; i < footCount; i++) {
            // figure out orientation and shit
            if (simGait(simTimeFuture, &orientationFuture, *workingSteps[i], gait[i], gaitCursor)) {
                delete workingSteps[i];
                workingSteps[i] = new StepFrame;
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
    float simTime = 0;
    
    struct StepFrame {
        float simTime = 0;
        float landTime = 0;
        float liftTime = 0;

        bool wasGrounded = false;
        Mat4 accumulatedGroundedOrientation = {}; 
        int groundedIterations = 0;
    };  
    deque<StepFrame>* steps;
    StepFrame** workingSteps;

    bool simGait(float time, Mat4* orientation, StepFrame& frame, Gait& gait, float cursor) {
        bool tookStep = false;
        bool isGrounded = gait.isGrounded(cursor);
        if (frame.wasGrounded < isGrounded) {
            // land
            frame.landTime = time;
        } else if (frame.wasGrounded > isGrounded) {
            // lift
            frame.liftTime = time;
            tookStep = true; 
        }   

        if (isGrounded) {
            // accumulate grounded orientation to avg later
            Mat4* ago = &frame.accumulatedGroundedOrientation;
            addm4(ago, orientation, ago);
            frame.groundedIterations++;
        }   

        frame.wasGrounded = isGrounded;
        frame.simTime = time;

        return tookStep;
    }
};

#endif /* WALKING_ROBOT_CLIP_H */

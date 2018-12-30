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

        this->lastSteps = new Step[footCount];
    }

    virtual ~WalkingRobotClip() {
        delete bodyChannel;
        delete[] footChannels;
    }

    void setTargets(Mat4* bodyOrientation, Vec3* feet) {
        this->bodyOrientation = bodyOrientation;
        this->feet = feet;
    }

    void sim(float delta) {
        float time = getTime();
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
        for (uint8_t i = 0; i < footCount; i++) {
            multm4v3(&orientationNow, &robot->getFeetHome()[i], 1, &wsFeetFuture[i]);
        }

        // clearing invalid predictions
        float dirtyTime = time - delta;
        for (uint8_t i = 0; i < footCount; i++) {
            auto it = steps[i].begin();
            while (it != steps[i].end()) {
                if (dirtyTime <= (*it).simTime) {
                    break;
                }
                it++;
            }
            steps[i].erase(it, steps[i].end());

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
                bool tookStep = simGait(time + future, &orientationFuture, frame, gait[i], cursor);
                if (tookStep) {
                    //cout << "new step at: " << simTime + future << endl;
                    // the step is complete, create a new working step
                    steps[i].emplace_back();
                }
            }

            //cout << "future: " << future << endl;
        } while (cursor < gaitCursor + 1);
        // after looping

        // calculate leg targets
        Vec3 wsTargets[footCount];
        Vec3 rsTargets[footCount];
        for (uint8_t i = 0; i < footCount; i++) {
            while (!steps[i].empty() && steps[i].front().simTime < time) {
                steps[i].pop_front(); // clear old steps
            }
            StepFrame& step = steps[i].front();

            getAvgPosition(step, &robot->getFeetHome()[i], &wsTargets[i]);

            robot->getMotionPlan()->orientationAt(step.landTime - time, &orientationFuture);
            inversem4(&orientationFuture, &invOrientationFuture);
            // TODO: invOrientationFuture future time needs to be time of footstep landing
            multm4v3(&invOrientationFuture, &wsTargets[i], 1, &rsTargets[i]);
        }

        // find new rs grounded foot position
        robot->getMotionPlan()->orientationAt(delta, &orientationFuture);
        inversem4(&orientationFuture, &invOrientationFuture);
        Vec3 wsFeetNow[footCount];
        Vec3 rsFeetFuture[footCount];
        for (uint8_t i = 0; i < footCount; i++) {
            multm4v3(&orientationNow, &robot->getFeet()[i], 1, &wsFeetNow[i]);
            multm4v3(&invOrientationFuture, &wsFeetNow[i], 1, &rsFeetFuture[i]);
        }

        float simTimeFuture = time + delta;
        // update tweens
        for (uint8_t i = 0; i < footCount; i++) {
            footChannels[i].clear();
            if (gait[i].isGrounded(gaitCursor)) {
                cout << "foot " << (int) i << " grounded" << endl;
                Vec3 handle = {0, 0, 0};

                Bezier3::Node startNode;
                setv3(&startNode.point, &robot->getFeet()[i]);
                setv3(&startNode.handle, &handle);

                Tween<Bezier3::Node> start(startNode, time, easeLinear);
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

                Bezier3::Node startNode;
                Vec3 rsPositionNow;
                multm4v3(&invOrientationNow, &lastSteps[i].position, 1, &rsPositionNow);
                setv3(&startNode.point, &rsPositionNow);
                setv3(&startNode.handle, &handle);
                Tween<Bezier3::Node> start(startNode, lastSteps[i].liftTime, easeQuadraticInOut);
                footChannels[i].insertTween(start);
                cout << "start: " << startNode.point.x << ", " << startNode.point.y << endl;

                Bezier3::Node endNode;
                setv3(&endNode.point, &rsTargets[i]);
                setv3(&endNode.handle, &handle);
                Tween<Bezier3::Node> end2(endNode, steps[i].front().landTime, easeNone);
                footChannels[i].insertTween(end2);
                cout << "end: " << endNode.point.x << ", " << endNode.point.y << endl;
            }
        }

        gaitCursor += delta; // delta * interval
        cout << "new cursor: " << gaitCursor << endl;

        for (uint8_t i = 0; i < footCount; i++) {
            // figure out orientation and shit
            if (simGait(simTimeFuture, &orientationFuture, *workingSteps[i], gait[i], gaitCursor)) {
                lastSteps[i].landTime = workingSteps[i]->landTime;
                lastSteps[i].liftTime = workingSteps[i]->liftTime;
                getAvgPosition(*workingSteps[i], &robot->getFeetHome()[i], &lastSteps[i].position);

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

    struct Step {
        float landTime = 0;
        float liftTime = 0;
        Vec3 position = {};
    };
    Step* lastSteps;

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

    void getAvgPosition(StepFrame& step, Vec3* home, Vec3* target) {
        Mat4* ago = &step.accumulatedGroundedOrientation;
        Mat4 avgGroundedOrientation;
        multm4s(ago, (1.0f / step.groundedIterations), &avgGroundedOrientation);
        multm4v3(&avgGroundedOrientation, home, 1, target);
    }
};

#endif /* WALKING_ROBOT_CLIP_H */

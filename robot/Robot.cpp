/* 
 * File:   Robot.cpp
 * Author: nyrmburk
 * 
 * Created on May 4, 2018, 9:07 PM
 */

#include "Robot.h"
#include "Config.h"
#include <algorithm>

Robot::Robot() {
    Config cfg("config/robot.json");

    body = new Body();
    cfg.getBody(body);

    int legsCount = body->legsCount;

    gait = new Gait[legsCount];
    cfg.getGait(gait, "alternating tripod");

    joints = new Joints[legsCount];
    feet = new Vec3[legsCount];

    identitym4(&orientation);

    steps = new deque<StepFrame>[legsCount];
    workingSteps = new StepFrame*[legsCount];


    for (uint8_t i = 0; i < legsCount; i++) {
        workingSteps[i] = new StepFrame;
    }
}

Robot::~Robot() {
    delete[] joints;
    delete[] feet;
}

Body* Robot::getBody() {
    return body;
}

World* Robot::getWorld() {
    return world;
}

void Robot::simulationStep(float now, float delta) {
    cout << "start simulation step" << endl;
    uint8_t legsCount = body->legsCount;
    // calculate per-foot speed

    // feet from past sim and future sim to calculate gait interval from greatest foot speed
    // ws means worldspace
    Vec3 wsFeetThen[legsCount];
    Vec3 wsFeetFuture[legsCount];

    // past and future orientations
    Mat4 orientationThen;
    Mat4 invOrientationThen;
    Mat4 orientationFuture;

    float future = 0; // delta between now and future sim
    float cursor = gaitCursor; // future sim cursor position

    // get the current orientation
    // this is actually useless lmao. I could have just used the robot orientation
    // use setm4 with orientation
    plan->orientationAt(0, &orientationFuture);
    // inverse used to convert robotspace to worldspace
    inversem4(&orientationFuture, &invOrientationThen);

    // get foot position at time now
    cout << "updating the foot home position" << endl;
    for (uint8_t i = 0; i < legsCount; i++) {
        multm4v3(&orientationFuture, &feetHome[i], 1, &wsFeetFuture[i]);
    }

    // clearing invalid predictions
    float dirtyTime = simTime;
    for (uint8_t i = 0; i < legsCount; i++) {
        if (!steps[i].empty() ) {
            StepFrame* frame = &steps[i].back();
            while (!steps[i].empty() && dirtyTime < frame->simTime) {
                steps[i].pop_back();
                frame = &steps[i].back();
            }
        }
        if (steps[i].empty()) {
            steps[i].push_back(*workingSteps[i]);
        }
    }

    cout << endl << endl << "predicting future..." << endl;
    do {
        //cout << endl << "start iteration: " << cursor << endl;
        future += delta;

        // get foot position in the future
        setm4(&orientationThen, &orientationFuture); // update the previous orientation
        plan->orientationAt(future, &orientationFuture); // calculate future orientation
        for (uint8_t i = 0; i < legsCount; i++) {
            setv3(&wsFeetThen[i], &wsFeetFuture[i]); // update the previous foot positions
            multm4v3(&orientationFuture, &feetHome[i], 1, &wsFeetFuture[i]);
        }
        
        // compare foot positions to get distance traveled
        float footSpeed = 0; 
        for (uint8_t i = 0; i < legsCount; i++) {
            Vec3 distance;
            subtractv3(&wsFeetFuture[i], &wsFeetThen[i], &distance);
            footSpeed = std::max(footSpeed, lengthv3(&distance));
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
        for (uint8_t i = 0; i < legsCount; i++) {
            StepFrame& frame = steps[i].back();
            bool tookStep = simGait(simTime + future, &orientationFuture, frame, gait[i], cursor);
            if (tookStep) {
                //cout << "new step at: " << simTime + future << endl;
                StepFrame newFrame;
                steps[i].push_back(newFrame);
            }
        }

        //cout << "future: " << future << endl;
    } while (cursor < gaitCursor + 1);
    // after looping

    gaitCursor += delta / future;
    cout << "new cursor: " << gaitCursor << endl;

    // calculate leg targets
    Vec3 wsTargets[legsCount];
    Vec3 rsTargets[legsCount];
    for (uint8_t i = 0; i < legsCount; i++) {
        StepFrame& step = steps[i].front();
        while (!steps[i].empty() && step.simTime < simTime) {
            steps[i].pop_front();
            step = steps[i].front();
        }
        Mat4* ago = &step.accumulatedGroundedOrientation;
        multm4s(ago, (1.0f / step.groundedIterations), ago);
        cout << "x: " << ago->m[12] << ", y: " << ago->m[13] << endl;
        cout << (int) i << ": land:" << step.landTime << ", lift:" << step.liftTime << endl;

        multm4v3(ago, &feetHome[i], 1, &wsTargets[i]);
        multm4v3(&invOrientationThen, &wsTargets[i], 1, &rsTargets[i]);
    }

    // update tweens
    for (uint8_t i = 0; i < legsCount; i++) {
        if (gait[i].isGrounded(gaitCursor)) {
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

    simTime += delta;
    for (uint8_t i = 0; i < legsCount; i++) {
        // figure out orientation and shit
        simGait(simTime, &orientationThen, *workingSteps[i], gait[i], gaitCursor);
    }
}

bool Robot::simGait(float time, Mat4* orientation, StepFrame& frame, Gait& gait, float cursor) {
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

void Robot::animationStep(float now, float delta) {
    if (this->currentAnimation) {
        this->currentAnimation->step(delta);
    }

    for (int i = 0; i < body->legsCount; i++) {
        body->legs[i].solveInverse(&bodyOrientation, &feet[i], &joints[i], nullptr);
    }

    plan->step(delta);
    plan->orientationAt(0, &orientation);
    //cout << orientation.m[12] << ", " << orientation.m[13] << endl;
}

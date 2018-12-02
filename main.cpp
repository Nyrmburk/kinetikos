/* 
 * File:   main.cpp
 * Author: nyrmburk
 *
 * Created on June 22, 2017, 3:48 PM
 */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "matrix/vec3.h"
#include "animation/Tween.h"
#include "animation/Vec3Channel.h"
#include "animation/Bezier3Channel.h"
#include "robot/Robot.h"
#include "network/Server.h"
#include "network/RobotProtocol.h"
#include "helper/SerializableTest.h"
#include "animation/AnimationJson.h"
#include "animation/RobotClip.h"

using namespace std;

int main(int argc, char** argv) {
    
    // test tweens
    Vec3 startPosition = {0, 0, 0};
    Tween<Vec3> start(startPosition, 0.0f, easeQuadraticInOut);
    
    Vec3 midPosition = {10, 5, 7};
    Tween<Vec3> mid(midPosition, 0.5f, easeLinear);
    
    Vec3 endPosition = {-5, 0, 3.141592654f};
    Tween<Vec3> end(endPosition, 0.9f, easeQuadraticOut);

    Vec3 futurePosition = {-1, -1, -1};
    Tween<Vec3> future(futurePosition, 1.8f, easeNone);

    Vec3 dupePosition = {1, 1, 1};
    Tween<Vec3> dupe(dupePosition, 1.8f, easeNone);
   
    Vec3 position;
    Vec3Channel positionChannel;
    
    positionChannel.insertTween(future);
    positionChannel.insertTween(dupe);
    positionChannel.insertTween(start);
    positionChannel.insertTween(mid);
    positionChannel.insertTween(end);
   
    int resolution = 20;
    float endTime = 2;
    for (int i = 0; i <= resolution; i++) {
        float time = ((float) i) / resolution;
        time *= endTime;
        positionChannel.act(time, position);
        cout << time << ": " << position.x << ", " << position.y << ", " << position.z << endl;
    }
    cout << endl;

    Bezier3Channel splineChannel;
  
    // test robot
    Robot robot;

    // test body
    Body* body = robot.getBody();
    Leg* legs = body->legs;
    int legsCount = body->legsCount;

    for (size_t i = 0; i < body->legsCount; i++) {
        Leg leg = legs[i];
        cout << "leg " << i << endl;
        cout << "offset: " << leg.offset.x << ", " << leg.offset.y << ", " << leg.offset.z << endl;
        cout << "coxa: " << leg.coxa.length << ", " << leg.coxa.angle << ", " << leg.coxa.range << endl;
        cout << "femur: " << leg.femur.length << ", " << leg.femur.angle << ", " << leg.femur.range << endl;
        cout << "tibia: " << leg.tibia.length << ", " << leg.tibia.angle << ", " << leg.tibia.range << endl;
    }
    cout << endl;
    
    // test joints
    Joints* joints = robot.getJoints();
    for (int i = 0; i < legsCount; i++) {
        cout << joints[i].joints[0] << ", " << joints[i].joints[1] << ", " << joints[i].joints[2] << endl;
    }
    cout << endl;

    // test feet
    Vec3* feet = robot.getFeet();
    for (int i = 0; i < legsCount; i++) {
        cout << feet[i].x << ", " << feet[i].y << ", " << feet[i].z << endl;
    }
    cout << endl;

    // test animations
    AnimationJson animations("config/animations.json");
    RobotClip clip = animations.getAnimation("actual");

    cout << "testing stepping" << endl;
    Mat4 cool;
    clip.setTargets(&cool, feet);
    float step = 0.2;
    for (float i = 0; i < clip.length; i += step) {
        clip.step(step);
        cout << feet[0].x << ", " << feet[0].y << ", " << feet[0].z << endl;
    }
    cout << endl;

    // test single object serialization
    Doggo heckin;

    char buffer[128];
    DataView view(buffer, 128);
    
    heckin.serialize(&view);
    cout << "total bytes: " << view.getIndex() << endl;
    view.rewind();
    heckin.deserialize(&view);
    heckin.print();
    cout << endl;

    // test nested object serialization
    view.mark();
    Spaceship whoosh;
    whoosh.serialize(&view);
    view.reset();
    whoosh.deserialize(&view);
    whoosh.passenger.print();
    cout << endl;

    // test server
    cout << "testing server" << endl;
    RobotProtocol protocol(robot);
    Server server(protocol);
    server.run([&robot](int millis) {
        robot.simulationStep(((float) millis) / 1000);
    }, 20);
    
    return 0;
}

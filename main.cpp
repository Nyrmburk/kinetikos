/* 
 * File:   main.cpp
 * Author: nyrmburk
 *
 * Created on June 22, 2017, 3:48 PM
 */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "tween/Tween.h"
#include "tween/Vec3Channel.h"
#include "robot/Robot.h"
#include "network/Server.h"
#include "helper/SerializableTest.h"
#include "control/Joints.h"
#include "control/Foot.h"
#include "control/FootControl.h"

using namespace std;

int main(int argc, char** argv) {
    
    // test tweens
    Vec3 startPosition = {0, 0, 0};
    Tween<Vec3> start(startPosition, 0.0f, &easeQuadraticInOut);
    
    Vec3 endPosition = {10, 5, 7};
    Tween<Vec3> end(endPosition, 1.0f, easeNone);
    
    Vec3 position;
    Vec3Channel positionChannel(position);
    
    positionChannel.insertTween(start);
    positionChannel.insertTween(end);
    
    for (int i = 0; i <= 10; i++) {
        positionChannel.step(((float) i) / 10);
        cout << position.x << ", " << position.y << ", " << position.z << endl;
    }
    cout << endl;
   
    // test footcontrol simulation step
    Robot robot;
    FootControl footControl = FootControl(6);
    footControl.step(&robot, (float) 3);

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

    // test joints
    Joints joints[6];
    for (int i = 0; i < 6; i++) {
        cout << joints[i].joints[0] << ", " << joints[i].joints[1] << ", " << joints[i].joints[2] << endl;
    }
    cout << endl;

    // test feet
    Foot feet[6];
    for (int i = 0; i < 6; i++) {
        cout << feet[i].foot.x << ", " << feet[i].foot.y << ", " << feet[i].foot.z << endl;
    }
    cout << endl;

    // test server
    Server server;
    server.run([&robot](int millis) {
        robot.simulationStep(((float) millis) / 1000);
    }, 20);
    
    return 0;
}

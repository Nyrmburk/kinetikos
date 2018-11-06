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
#include "control/Joint.h"
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
    std::cout << std::endl;
   
    // test footcontrol simulation step
    Robot robot;
    FootControl footControl = FootControl(6);
    footControl.step(&robot, (float) 3);

    // test single object serialization
    Doggo heckin;

    char buffer[128];
    DataView view(buffer, 128);
    
    heckin.serialize(&view);
    std::cout << "total bytes: " << view.getIndex() << std::endl;
    view.rewind();
    heckin.deserialize(&view);
    view.rewind();
    heckin.print();
    std::cout << std::endl;

    // test nested object serialization
    Spaceship whoosh;
    whoosh.serialize(&view);
    view.rewind();
    whoosh.deserialize(&view);
    whoosh.passenger.print();
    std::cout << std::endl;

    // test joints
    Joint joints[6];
    for (int i = 0; i < 6; i++) {
        std::cout << joints[i].joint << std::endl;
    }
    std::cout << std::endl;

    // test feet
    Foot feet[6];
    for (int i = 0; i < 6; i++) {
        std::cout << feet[i].foot.x << ", " << feet[i].foot.y << ", " << feet[i].foot.z << std::endl;
    }
    std::cout << std::endl;

    // test server
    Server server;
    server.run([&robot](int millis) {
        robot.simulationStep(((float) millis) / 1000);
    }, 20);
    
    return 0;
}

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
#include "control/FootControl.h"
#include "network/Server.h"

using namespace std;

int main(int argc, char** argv) {
    
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
    
    Robot robot;
    FootControl footControl = FootControl(6);
    footControl.step(&robot, (float) 3);

    Server server;
    server.run();
    
    return 0;
}

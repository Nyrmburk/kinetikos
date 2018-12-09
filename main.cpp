/* 
 * File:   main.cpp
 * Author: nyrmburk
 *
 * Created on June 22, 2017, 3:48 PM
 */

#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "robot/Robot.h"
#include "network/Server.h"
#include "network/RobotProtocol.h"
#include "animation/AnimationJson.h"
#include "animation/RobotClip.h"

using namespace std;

int main(int argc, char** argv) {
 
    cout << "loading robot config" << endl;
    Robot robot;

    cout << "loading robot animations" << endl;
    AnimationJson animations("config/animations.json");
    RobotClip home = animations.getAnimation("home");
    home.setTargets(robot.getOrientation(), robot.getFeet());
    robot.setAnimation(&home);

    cout << "initializing server" << endl;
    Server server(robot);

    server.addTimer([&](int millis) {
        float delta = ((float) millis / 1000);
        robot.simulationStep(delta);
    }, 60);
    
    server.addTimer([&](int millis) {
        float delta = ((float) millis / 1000);
        robot.animationStep(delta);
    }, 20);

    server.addTimer([&](int millis) {
        server.publishJoints();
        server.publishFeet();
        server.publishOrientation();
    }, 20);

    cout << "starting server" << endl;
    server.run();
    return 0;
}

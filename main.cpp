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
#include "robot/Config.h"
#include "network/Server.h"
#include "network/RobotProtocol.h"
#include "animation/AnimationJson.h"
#include "animation/RobotClip.h"
#include "animation/WalkingRobotClip.h"
#include "../mapping/JoystickPlan.h"
#include "../matrix/vec2.h"

using namespace std;

int main(int argc, char** argv) {

    cout << "loading robot config" << endl;
    Robot robot;

    cout << "loading robot animations" << endl;
    AnimationJson homeAnimation("config/animations/home.json");
    RobotClip home = homeAnimation.getAnimation();
    Mat4 orientation;
    Vec3 feetHome[robot.getBody()->legsCount];
    home.setTargets(&orientation, feetHome);
    home.step(0);
    home.setTargets(robot.getBodyOrientation(), robot.getFeet());
    home.step(0);
    home.setTargets(robot.getBodyOrientation(), robot.getFeet());
    robot.setFeetHome(feetHome);
    robot.setAnimation(&home);
    
    AnimationJson demoAnimation("config/animations/demo.json");
    RobotClip demo = demoAnimation.getAnimation();
    demo.setTargets(robot.getBodyOrientation(), robot.getFeet());
    robot.setAnimation(&demo);

    Config cfg("config/robot.json");
    Gait gait[robot.getBody()->legsCount];
    cfg.getGait(gait, "alternating tripod");
    //cfg.getGait(gait, "smooth tripod");
    //cfg.getGait(gait, "ripple");
    WalkingRobotClip walk(&robot, gait);
    walk.setTargets(robot.getBodyOrientation(), robot.getFeet());
    robot.setAnimation(&walk);

    JoystickPlan plan(300, 10, 1, 0);
    robot.setMotionPlan(&plan);

    cout << "initializing server" << endl;
    Server server(robot);
    server.joystickPlan = &plan;

    server.addTimer([&](int delta) {
        float fDelta = ((float) delta) / 1000;
        robot.simulationStep(fDelta);
        walk.sim(fDelta);
    }, 20);

    server.addTimer([&](int delta) {
        float fDelta = ((float) delta) / 1000;
        robot.animationStep(fDelta);
    }, 20);

    server.addTimer([&](int delta) {
        server.publishBodyOrientation();
        server.publishJoints();
        server.publishFeet();
        server.publishOrientation();
    }, 20);

    /*
    unsigned long long lastBytesRead = 0;
    unsigned long long lastBytesWritten = 0;
    server.addTimer([&](int millis) {
        cout << "read  " <<
                (server.getTotalBytesRead() - lastBytesRead) / 1024 << " kBps" << endl;
        cout << "wrote " <<
                (server.getTotalBytesWritten() - lastBytesWritten) / 1024 << " kBps" << endl;
        lastBytesRead = server.getTotalBytesRead();
        lastBytesWritten = server.getTotalBytesWritten();
    }, 1000);
    */

    cout << "starting server" << endl;
    server.run();
    return 0;
}

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
#include "network/H264Server.h"
#include "network/RobotProtocol.h"
#include "animation/AnimationJson.h"
#include "animation/RobotClip.h"
#include "animation/WalkingRobotClip.h"
#include "../mapping/JoystickPlan.h"
#include "../matrix/vec2.h"
#include "../helper/File.h"
#include "../control/PiPololuMotorControl.h"

using namespace std;

int main(int argc, char** argv) {
    cout << "loading robot config" << endl;
    Config config(origin() + "/config/robot.json");
    Robot robot(config);

    int childpid = fork();
    if (childpid < 0) {
        cout << "failure forking, no livestream server started" << endl;
    } else if (childpid == 0) {
        // child process
        cout << "starting livestream server" << endl;
        H264Server livestream;
        livestream.run();
        cout << "livestream server died" << endl;
        return 0;
    }

    int fd = PiPololuMotorControl::getSerial();
    if (fd >= 0) {
        cout << "using Pololu Mini Maestro motor controller" << endl;
        robot.setMotorControl(new PiPololuMotorControl(fd));
    } else {
        cout << "not using a motor controller" << endl;
    }

    int legsCount = robot.getBody()->legsCount;
    cout << "loading robot animations" << endl;
    AnimationJson homeAnimation(origin() + "/config/animations/home.json");
    RobotClip home = homeAnimation.getAnimation();
    Mat4 orientation;
    Vec3 feetHome[legsCount];
    home.setTargets(&orientation, feetHome);
    home.step(0);
    home.setTargets(robot.getBodyOrientation(), robot.getFeet());
    home.step(0);
    home.setTargets(robot.getBodyOrientation(), robot.getFeet());
    robot.setFeetHome(feetHome);
    robot.setAnimation(&home);
    
    AnimationJson demoAnimation(origin() + "/config/animations/startup.json");
    RobotClip demo = demoAnimation.getAnimation();
    demo.setTargets(robot.getBodyOrientation(), robot.getFeet());
    robot.setAnimation(&demo);

    Gait gait[legsCount];
    config.getGait(gait, "alternating tripod");
    //config.getGait(gait, "smooth tripod");
    //config.getGait(gait, "ripple");
    WalkingRobotClip walk(&robot, gait);
    walk.setTargets(robot.getBodyOrientation(), robot.getFeet());

    JoystickPlan plan(300, 10, 2, 0);
    robot.setMotionPlan(&plan);

    cout << "initializing robot server" << endl;
    Server server(robot);

    server.addTimer([&](int delta) {
        robot.setAnimation(&walk);
        server.joystickPlan = &plan;
    }, 5000, 0, 1);

    // simulate
    server.addTimer([&](int delta) {
        float fDelta = ((float) delta) / 1000;
        robot.simulationStep(fDelta);
        walk.sim(fDelta);
    }, 0, 20);

    // animate
    server.addTimer([&](int delta) {
        float fDelta = ((float) delta) / 1000;
        robot.animationStep(fDelta);
        for (int i = 0; i < legsCount; i++) {
//            robot.getJoints()[i].coxa = 0.5;
//            robot.getJoints()[i].femur = i < 3;
//            robot.getJoints()[i].tibia = 0.5;
        }
        MotorControl* motorControl = robot.getMotorControl();
        if (motorControl) {
            motorControl->setMotors(robot.getJoints(), legsCount);
        }
    }, 0, 20);

    // server publish data to clients
    server.addTimer([&](int delta) {
        server.publishBodyOrientation();
        server.publishJoints();
        server.publishFeet();
        server.publishOrientation();
    }, 0, 20);

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
    }, 1000, 1000);
    */

    cout << "starting robot server" << endl;
    server.run();
    return 0;
}

#ifndef ROBOT_PROTOCOL_H
#define ROBOT_PROTOCOL_H

#include "../robot/Robot.h"
#include "Protocol.h"

class RobotProtocol : public Protocol {
public:
    RobotProtocol(Robot& robot) : robot(robot) {}

    Robot& robot;

    // I need to turn my functions into functors. after all, that's how I'm using them
    // control
    void controlJoints(DataView& in, DataView& out);
    void controlFeet(DataView& in, DataView& out);
    void controlFootPaths(DataView& in, DataView& out);
    void controlVelocity(DataView& in, DataView& out);
    void controlNavigationPath(DataView& in, DataView& out);
    void controlDestination(DataView& in, DataView& out);

    // parameters
    void getBody(DataView& in, DataView& out) {
        out.writeSerial(robot.getBody());
    }
    void isThisAPigeon(DataView& in, DataView& out) {
    }
    // states

    void echo(DataView& in, DataView& out);

    enum Control {
        joints = 200,
        feet,
        footPaths,
        velocity,
        navigation,
        destination,
    };

    enum Parameters {
        body = 100,
        pigeon = 300,
    };

    void handle(uint16_t controlCode, DataView& in, DataView& out) {
        switch (controlCode) {
            case Control::joints: return controlJoints(in, out);
            case Control::feet: return controlFeet(in, out);
            case Control::footPaths: return controlFootPaths(in, out);
            case Control::velocity: return controlVelocity(in, out);
            case Control::navigation: return controlNavigationPath(in, out);
            case Control::destination: return controlDestination(in, out);
            case Parameters::body: return getBody(in, out);
            case Parameters::pigeon: return isThisAPigeon(in, out);
            case 0x4141: return echo(in, out);
        }
    }
};

#endif /* ROBOT_PROTOCOL_H */

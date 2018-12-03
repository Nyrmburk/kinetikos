#ifndef ROBOT_PROTOCOL_H
#define ROBOT_PROTOCOL_H

#include "../robot/Robot.h"
#include "Protocol.h"

template <typename T>
class RobotProtocol : public Protocol<T> {
public:
    RobotProtocol(Robot& robot) : robot(robot) {}

    Robot& robot;

    // I need to turn my functions into functors. after all, that's how I'm using them
    // control
    void controlJoints(T& remote, uint16_t opcode, DataView& data);
    void controlFeet(T& remote, uint16_t opcode, DataView& data);
    void controlFootPaths(T& remote, uint16_t opcode, DataView& data);
    void controlVelocity(T& remote, uint16_t opcode, DataView& data);
    void controlNavigationPath(T& remote, uint16_t opcode, DataView& data);
    void controlDestination(T& remote, uint16_t opcode, DataView& data);

    // parameters
    void getBody(T& remote, uint16_t opcode, DataView& data) {
        this->send(remote, opcode, robot.getBody());
    }
    void isThisAPigeon(T& remote, uint16_t opcode, DataView& data) {
    }
    // states

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

    void handle(T& remote, uint16_t opcode, DataView& data) {
        switch (opcode) {
            case Control::joints: return controlJoints(remote, opcode, data);
            case Control::feet: return controlFeet(remote, opcode, data);
            case Control::footPaths: return controlFootPaths(remote, opcode, data);
            case Control::velocity: return controlVelocity(remote, opcode, data);
            case Control::navigation: return controlNavigationPath(remote, opcode, data);
            case Control::destination: return controlDestination(remote, opcode, data);
            case Parameters::body: return getBody(remote, opcode, data);
            case Parameters::pigeon: return isThisAPigeon(remote, opcode, data);
        }
    }
};

#include "ControlProtocol.tpp"

#endif /* ROBOT_PROTOCOL_H */

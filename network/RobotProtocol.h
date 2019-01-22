#ifndef ROBOT_PROTOCOL_H
#define ROBOT_PROTOCOL_H

#include "../robot/Robot.h"
#include "Protocol.h"
#include "../mapping/JoystickPlan.h"

template <typename T>
class RobotProtocol : public Protocol<T> {
public:
    RobotProtocol(Robot& robot) : robot(robot) {}

    Robot& robot;
    JoystickPlan* joystickPlan;

    // control
    void controlBodyOrientation(T& remote, uint16_t opcode, DataView& data);
    void controlJoints(T& remote, uint16_t opcode, DataView& data);
    void controlFeet(T& remote, uint16_t opcode, DataView& data);
    void controlFootPaths(T& remote, uint16_t opcode, DataView& data);
    void controlOrientation(T& remote, uint16_t opcode, DataView& data);
    void controlVelocity(T& remote, uint16_t opcode, DataView& data);
    void controlNavigationPath(T& remote, uint16_t opcode, DataView& data);
    void controlDestination(T& remote, uint16_t opcode, DataView& data);
    void controlJoystick(T& remote, uint16_t opcode, DataView& data);

    // parameters
    void getBody(T& remote, uint16_t opcode, DataView& data) {
        this->send(remote, opcode, robot.getBody());
    }

    void getWorkspaces(T& remote, uint16_t opcode, DataView& data) {
        this->send(remote, opcode, [&](DataView& out){
            for (int i = 0; i < robot.getBody()->legsCount; i++) {
                robot.getWorkspaces()[i].serialize(&out);
            }
        });
    }

    void isThisAPigeon(T& remote, uint16_t opcode, DataView& data) {
    }
    // states

    enum Control {
        bodyOrientation = 200,
        joints,
        feet,
        footPaths,
        orientation,
        velocity,
        navigation,
        destination,
        joystick,
    };

    enum Parameters {
        body = 100,
        workspaces,
        pigeon = 300,
    };

    void handle(T& remote, uint16_t opcode, DataView& data) {
        switch (opcode) {
            case Control::bodyOrientation: return controlBodyOrientation(remote, opcode, data);
            case Control::joints: return controlJoints(remote, opcode, data);
            case Control::feet: return controlFeet(remote, opcode, data);
            case Control::footPaths: return controlFootPaths(remote, opcode, data);
            case Control::orientation: return controlOrientation(remote, opcode, data);
            case Control::velocity: return controlVelocity(remote, opcode, data);
            case Control::navigation: return controlNavigationPath(remote, opcode, data);
            case Control::destination: return controlDestination(remote, opcode, data);
            case Control::joystick: return controlJoystick(remote, opcode, data);
            case Parameters::body: return getBody(remote, opcode, data);
            case Parameters::workspaces: return getWorkspaces(remote, opcode, data);
            case Parameters::pigeon: return isThisAPigeon(remote, opcode, data);
        }
    }

    struct RobotUserData {
        JoystickPlan::JoystickPair* joysticks = nullptr;
    };

    void onConnect(T& remote) {
        Protocol<T>::onConnect(remote);
        RobotUserData* data = new RobotUserData();
        this->setUserData(remote, data);
    }

    void onDisconnect(T& remote) {
        Protocol<T>::onDisconnect(remote);
        RobotUserData* data = (RobotUserData*)this->getUserData(remote);
        if (joystickPlan && data->joysticks) {
            joystickPlan->removeChannel(data->joysticks);
        }
        delete data->joysticks;
        delete data;
    }
    
    void publishBodyOrientation() {
        this->publish(Control::bodyOrientation, Control::bodyOrientation, [&](DataView& out) {
            serializem4(&out, robot.getBodyOrientation());
        });
    }

    void publishJoints() {
        this->publish(Control::joints, Control::joints, [&](DataView& out) {
            for (uint8_t i = 0; i < robot.getBody()->legsCount; i++) {
                out.writeSerial(&robot.getJoints()[i]);
            }
        });
    }
    
    void publishFeet() {
        this->publish(Control::feet, Control::feet, [&](DataView& out) {
            for (uint8_t i = 0; i < robot.getBody()->legsCount; i++) {
                serializev3(&out, &robot.getFeet()[i]);
            }
        });
    }

    void publishOrientation() {
        this->publish(Control::orientation, Control::orientation, [&](DataView& out){
            serializem4(&out, robot.getOrientation());
        });
    }
};

#include "ControlProtocol.tpp"

#endif /* ROBOT_PROTOCOL_H */

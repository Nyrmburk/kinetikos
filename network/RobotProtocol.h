#include "Protocol.h"

// control
Serializable* controlJoints(DataView* it);
Serializable* controlFeet(DataView* it);
Serializable* controlFootPaths(DataView* it);
Serializable* controlVelocity(DataView* it);
Serializable* controlNavigationPath(DataView* it);
Serializable* controlDestination(DataView* it);

// parameters
Serializable* isThisAPigeon(DataView* it) {
    return nullptr;
}
// states

Serializable* echo(DataView* it) {
    return nullptr;
}

enum Control {
    joints = 100,
    feet,
    footPaths,
    velocity,
    navigation,
    destination,
};

enum Parameters {
    pigeon = 200,
};

Protocol::Protocol() {

    addProcedure(Control::joints, controlJoints);
    addProcedure(Control::feet, controlJoints);
    addProcedure(Control::footPaths, controlJoints);
    addProcedure(Control::velocity, controlJoints);
    addProcedure(Control::navigation, controlJoints);
    addProcedure(Control::destination, controlJoints);

    addProcedure(Parameters::pigeon, isThisAPigeon);

    //AA
    addProcedure(0x4141, echo);
}


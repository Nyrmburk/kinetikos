#include "Protocol.h"

// control
Serializable* controlJoints(Readerator* it);
Serializable* controlFeet(Readerator* it);
Serializable* controlFootPaths(Readerator* it);
Serializable* controlVelocity(Readerator* it);
Serializable* controlNavigationPath(Readerator* it);
Serializable* controlDestination(Readerator* it);

// parameters
Serializable* isThisAPigeon(Readerator* it) {
    return nullptr;
}
// states

Serializable* echo(Readerator* it) {
    return nullptr;
}

Protocol::Protocol() {

    Procedure controls[] {
        controlJoints,
        controlFeet,
        controlFootPaths,
        controlVelocity,
        controlNavigationPath,
        controlDestination,
        nullptr
    };
    insert(100, controls);

    Procedure parameters[] {
        isThisAPigeon,
        nullptr
    };
    insert(200, parameters);

    Procedure whataboutsingles[] {
        echo,
        nullptr
    };
    //AA
    insert(0x4141, echo);
}


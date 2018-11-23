#include "FootPath.h"
#include "../matrix/vec3.h"
#include "../animation/Tween.h"

void FootPathControl::control(Robot* robot, const Vec3* target, Bezier3Channel* returnPath) {
    Vec3* handle = new Vec3;
    setv3s(0, 1, 0, handle);

    Bezier3::Node* node = new Bezier3::Node;
    node->point = const_cast<Vec3*>(target);
    node->handle = handle;

    Tween<Bezier3::Node> tween(*node, 0, easeLinear);
    returnPath->insertTween(tween);

    // so that's fine and dandy, but there's obvious concerns here
    // where do I put the actual tween in the channel (timestamp)?
    //     I should just pass the timestamp
    // how do I manage it so that I can update the time later?
    //     don't?
    //     don't return a channel, return a tween?
    // what if there is already some animation running with future animations planned?
    //     cancel all the tweens after the current timestamp? <-- yes
    //         this could result in some jerky behavior
    //             leave that up to the caller?
    //             I could have some daemon that looks at the paths for impossible behavior?
    //     wait until there's nothing queued?
    //     add onto the queue?
    // how do I make the foot path go up?
    //     follow a spline?
    //     generate a spline precedurally based on a video I lost
    //         it's the siggraph 2018 procedural climbing animation video
    // how do I plan the path so that I don't crash the foot into the environment?
    //     plan the spline from the map? ^^^
}

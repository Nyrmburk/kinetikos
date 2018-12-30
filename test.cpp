#include "test.h"

using namespace std;

int test() {
    
    // test tweens
    Vec3 startPosition = {0, 0, 0};
    Tween<Vec3> start(startPosition, 0.0f, easeQuadraticInOut);
    
    Vec3 midPosition = {10, 5, 7};
    Tween<Vec3> mid(midPosition, 0.5f, easeLinear);
    
    Vec3 endPosition = {-5, 0, 3.141592654f};
    Tween<Vec3> end(endPosition, 0.9f, easeQuadraticOut);

    Vec3 futurePosition = {-1, -1, -1};
    Tween<Vec3> future(futurePosition, 1.8f, easeNone);

    Vec3 dupePosition = {1, 1, 1};
    Tween<Vec3> dupe(dupePosition, 1.8f, easeNone);
   
    Vec3 position;
    Vec3Channel positionChannel;
    
    positionChannel.insertTween(future);
    positionChannel.insertTween(dupe);
    positionChannel.insertTween(start);
    positionChannel.insertTween(mid);
    positionChannel.insertTween(end);
   
    int resolution = 20;
    float endTime = 2;
    for (int i = 0; i <= resolution; i++) {
        float time = ((float) i) / resolution;
        time *= endTime;
        positionChannel.act(time, position);
        cout << time << ": " << position.x << ", " << position.y << ", " << position.z << endl;
    }
    cout << endl;

    Bezier3Channel splineChannel;
  
    // test robot
    Robot robot;

    // test body
    Body* body = robot.getBody();
    Leg* legs = body->legs;
    int legsCount = body->legsCount;

    for (size_t i = 0; i < body->legsCount; i++) {
        Leg leg = legs[i];
        cout << "leg " << i << endl;
        cout << "offset: " << leg.offset.x << ", " << leg.offset.y << ", " << leg.offset.z << endl;
        cout << "coxa: " << leg.coxa.length << ", " << leg.coxa.angle << ", " << leg.coxa.range << endl;
        cout << "femur: " << leg.femur.length << ", " << leg.femur.angle << ", " << leg.femur.range << endl;
        cout << "tibia: " << leg.tibia.length << ", " << leg.tibia.angle << ", " << leg.tibia.range << endl;
    }
    cout << endl;
    
    // test joints
    Joints* joints = robot.getJoints();
    for (int i = 0; i < legsCount; i++) {
        cout << joints[i].coxa << ", " << joints[i].femur << ", " << joints[i].tibia << endl;
    }
    cout << endl;

    // test feet
    Vec3* feet = robot.getFeet();
    for (int i = 0; i < legsCount; i++) {
        cout << feet[i].x << ", " << feet[i].y << ", " << feet[i].z << endl;
    }
    cout << endl;

    // test animations
    AnimationJson animation("config/animations/demo.json");
    RobotClip clip = animation.getAnimation();

    cout << "testing stepping" << endl;
    Mat4 cool;
    clip.setTargets(&cool, feet);
    float step = 0.2;
    for (float i = 0; i < clip.length; i += step) {
        clip.step(step);
        cout << feet[0].x << ", " << feet[0].y << ", " << feet[0].z << endl;
    }
    cout << endl;

    // test single object serialization
    Doggo heckin;

    char buffer[128];
    DataView view(buffer, 128);
    
    heckin.serialize(&view);
    cout << "total bytes: " << view.getIndex() << endl;
    view.rewind();
    heckin.deserialize(&view);
    heckin.print();
    cout << endl;

    // test nested object serialization
    view.mark();
    Spaceship whoosh;
    whoosh.serialize(&view);
    view.reset();
    whoosh.deserialize(&view);
    whoosh.passenger.print();
    cout << endl;

    // test setting home animation
    RobotClip home = animation.getAnimation();
    home.setTargets(robot.getOrientation(), feet);
    home.step(0);

    // test server
    cout << "testing server" << endl;
    Server server(robot);
    float j = 0;
    server.addTimer([&](int delta) {
        float fDelta = ((float) delta) / 1000;
        robot.simulationStep(fDelta);

        home.step(delta);

        j += 0.05f;
        //robot.getOrientation()->m[14] = 30 * sin(j) + 30;

        /*
        for (int i = 0; i < legsCount; i++) {
            joints[i].coxa = (1 + sin(j)) / 2;
            joints[i].femur = (1 + sin(j)) / 2;
            joints[i].tibia = (1 + sin(j)) / 2;

            body->legs[i].solveForward(robot.getOrientation(), &joints[i], &feet[i]);
            cout << i << "::" << feet[i].x << ":" << feet[i].y << ":" << feet[i].z << endl;
        }
        //cout << endl;
        */
//        /*
        for (int i = 0; i < legsCount; i++) {
            body->legs[i].solveInverse(robot.getOrientation(), &feet[i], &joints[i], nullptr);
        }
//        */

        server.publish(server.Control::joints, server.Control::joints, [&](DataView& out){
            for (int i = 0; i < legsCount; i++) {
                out.writeSerial(&joints[i]);
            }
        });

        server.publish(server.Control::feet, server.Control::feet, [&](DataView& out){
            for (int i = 0; i < legsCount; i++) {
                serializev3(&out, &feet[i]);
            }
        });

        server.publish(server.Control::orientation, server.Control::orientation, [&](DataView& out){
            serializem4(&out, robot.getOrientation());
        });
    }, 20);

    server.run();
    return 0;
}

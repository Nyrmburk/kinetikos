/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Robot.h
 * Author: nyrmburk
 *
 * Created on May 4, 2018, 9:07 PM
 */

#ifndef ROBOT_H
#define ROBOT_H

#include "Body.h"
#include "../mapping/World.h"
#include "../control/Foot.h"
#include "../control/Joints.h"
#include "../control/MotorControl.h"

class Foot;
class FootControl;
class Joints;
class JointsControl;
class MotorControl;

class Robot {
public:
    Robot();
    virtual ~Robot();
    
    Body* getBody();
    World* getWorld();
    
    void simulationStep(float delta);
    void animationStep(float delta);
private:
    Body *body;
    World *world;
    
    Joints* joints;
    Foot* feet;

    MotorControl *motorControl;
    FootControl *footControl;
};

#endif /* ROBOT_H */

/*
include "../matrix/Mat4.fbs";
include "../matrix/Vec3.fbs";

struct Joint {
	angle:float;
	trim:short;
}

table Foot {
	position:Vec3;
//	tween:Tween;
}

table RobotState {
	// world space
	transformation:Mat4;
	// local space
	// layer 0
	joints:[Joint];
	trim:[short];
	// layer 1
	feet:[Foot];
	// layer 2
	velocity:float;
	acceleration:float;
	damping:float;
	angularVelocity:float;
	angularAcceleration:float;
	angularDamping:float;
	// layer 3
//	navigationTarget:NavigationTarget; // layer 3
}
root_type RobotState;
*/

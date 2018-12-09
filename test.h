/* 
 * File:   main.cpp
 * Author: nyrmburk
 *
 * Created on June 22, 2017, 3:48 PM
 */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cmath>

#include "matrix/vec3.h"
#include "animation/Tween.h"
#include "animation/Vec3Channel.h"
#include "animation/Bezier3Channel.h"
#include "robot/Robot.h"
#include "network/Server.h"
#include "network/RobotProtocol.h"
#include "helper/SerializableTest.h"
#include "animation/AnimationJson.h"
#include "animation/RobotClip.h"

int test();

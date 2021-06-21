#ifndef ROBOT_M1_H
#define ROBOT_M1_H

#include <list>
#include "src/Environment/Environment.h"
#include "src/Robot/Robot.h"
#include "src/Robot/Bumper.h"


class Robot_M1: public Robot
{
public:
    Robot_M1(Environment *environment);
    Robot_M1(Environment *environment, string name, float batteryCharge);

    // Methodes
    void clean();
    void moveRobot();

private:
    // Var
    Robot *_robot;
    Environment *_environment;
    Bumper *_bumper;
    int _batteryCharge;
    Position _robotPos;
};

#endif // ROBOT_M1_H

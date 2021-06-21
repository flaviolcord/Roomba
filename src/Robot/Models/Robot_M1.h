#ifndef ROBOT_M1_H
#define ROBOT_M1_H

#include <list>
#include "src/Environment/Environment.h"
#include "src/Robot/Robot.h"
#include "src/Robot/Bumper.h"


class Robot_M1: public Robot
{
public:
    // Class constructors
    Robot_M1(Environment *environment);
    Robot_M1(Environment *environment, string name, float batteryCharge);

    // Methods
    void clean();


private:
    // Var
    Robot *_robot;
    Environment *_environment;
    Bumper *_bumper;
    int _batteryCharge;
    Position _robotPos;

    // Methods
    void moveRobot();
    void returnRobotToStation();
    void checkPos(Position &pos);
};

#endif // ROBOT_M1_H

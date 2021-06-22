#ifndef ROBOT_M1_H
#define ROBOT_M1_H

#include <list>
#include "Environment/Environment.h"
#include "Robot/Robot.h"
#include "Robot/Bumper.h"


class Robot_M1: public Robot
{
public:
    // Class constructors
    Robot_M1(Environment *environment);
    Robot_M1(Environment *environment, string name, float batteryCharge);

    // Methods
    void clean();
    int getCellClean() {return _cellClean;}

private:
    // Var
    Environment *_environment;
    Bumper *_bumper;
    int _batteryCharge;
    int _cellClean;

    // Methods
    void moveRobot();
    void returnRobotToStation();
    void checkPos(Position &pos);
};

#endif // ROBOT_M1_H

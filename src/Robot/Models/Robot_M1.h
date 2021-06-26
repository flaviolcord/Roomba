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
    Robot_M1(Environment *environment, string name, int batteryCapacity);
    Robot_M1(Environment *environment, string file_name);

    // Methods
    void clean();

private:
    // Var
    Environment *_environment;
    Bumper *_bumper;
    int _batteryCapacity;
    int _cellClean;

    // Methods
    void moveRobot();
    void returnRobotToStation();
    void checkPos(Position &pos);
};

#endif // ROBOT_M1_H

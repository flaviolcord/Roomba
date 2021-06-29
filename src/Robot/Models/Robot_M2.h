#ifndef ROBOT_M2_H
#define ROBOT_M2_H

#include <list>
#include "Environment/Environment.h"
#include "Robot/Laser.h"
#include "Robot/Robot.h"

class Robot_M2: public Robot
{
public:
    // Class constructors
    Robot_M2(Environment *environment);
    Robot_M2(Environment *environment, string name, int batteryCapacity);
    Robot_M2(Environment *environment, string file_name);

    // Getters
    float getOrientation(){return _orientation;};
    // Methods
    virtual void clean();

private:
     // Var
    Environment *_environment;
    Laser *_laser;
    int _batteryCapacity;
    int _cellClean;
    float _orientation;

    // Methods
    void moveRobot();
    void returnRobotToStation();
    void checkPos(Position &pos);
    void turn_robot(Position old_pos, Position new_pos);
};

#endif // ROBOT_M2_H
#ifndef ROBOT_H
#define ROBOT_H

#include "Environment/Environment.h"
#include "Robot/Battery.h"

class Robot
{
public:
    // Class constructors
    Robot(Environment *environment);
    Robot(Environment *environment, string name, int batteryCapacity);
    Robot(Environment *environment, string fileName);

    // Virtual methods
    virtual void clean()=0;

    // Methods
    void updateBattery(int value, bool charge);
    bool batteryDischarged();

    // Getters
    Position getRobotPos ();
    int getBatteryValue() {return _battery->getLevel();}

    // Setters
    void setRobotPos (Position pos) {_currentPos = pos;}
    Environment* getEvironment() { return _environment;}
    //virtual void clean();

private:
    // Var
    string _name;
    Position _currentPos;
    Environment *_environment;

    // Battery
    Battery *_battery;
    int _batteryCapacity;
    list<string> _fileLines;

    // Virtual methods
    virtual void moveRobot()=0;
    virtual void returnRobotToStation()=0;

};

#endif // ROBOT_H

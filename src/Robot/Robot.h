#ifndef ROBOT_H
#define ROBOT_H

#include "src/Environment/Environment.h"
#include "src/Robot/Battery.h"

class Robot
{
public:
    // Class constructors
    Robot(Environment *environment);
    Robot(Environment *environment, string name, int batteryCharge);
    Robot(Environment *environment, string fileName);

    // Methods
    void updateBattery(int value, bool charge);
    bool batteryDischarged();

    // Getters
    Position getRobotPos ();
    int getBatteryValue() {return _battery->getLevel();}

    // Setters
    void setRobotPos (Position pos) {_currentPos = pos;}
    Environment* getEvironment() { return _environment;}

private:
    // Var
    string _name;
    Position _currentPos;
    Environment *_environment;

    // Battery
    Battery *_battery;
    int _batteryCharge;

};

#endif // ROBOT_H

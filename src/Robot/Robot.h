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
    Robot(string fileName);

    // Methodes
    void updateBattery(int value, bool charge);
    bool batteryDischarged();

    // getters
    Position getRobotPos ();
    int getBatteryValue() {return _battery->getLevel();}

    // setters
    void setRobotPos (Position pos) {_currentPos = pos;}

private:
    // var
    string _name;
    Position _currentPos;
    Environment *_environment;

    // mudar para o objeto Battery
    Battery *_battery;
    int _batteryCharge;

};

#endif // ROBOT_H

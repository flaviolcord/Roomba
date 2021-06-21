#include "Robot.h"

#define DEFAULT_BATTERY 100

Robot::Robot(Environment *environment)
{
    _environment = environment;
    _name = "default_name";
    _battery = new Battery(DEFAULT_BATTERY);
    setRobotPos(environment->getStationPos());
}

Robot::Robot(Environment *environment, string name, int batteryCharge)
{
    _name = name;
    _battery = new Battery(batteryCharge);
    _environment = environment;
    setRobotPos(environment->getStationPos());
}

Robot::Robot(string fileName)
{
    //ctor
}

void Robot::updateBattery(int value, bool charge)
{
    if(charge) {
        _battery->chargeBattery(value);

    } else {
        _battery->dischargeBattery();
    }
}

bool Robot::batteryDischarged()
{
    return (_battery->getLevel() == 0);
}

Position Robot::getRobotPos()
{
    return _currentPos;
}


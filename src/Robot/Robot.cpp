#include "Robot.h"
#include <fstream>

#define DEFAULT_BATTERY 100

Robot::Robot(Environment *environment)
{
    _environment = environment;
    _name = "default_name";
    _battery = new Battery(DEFAULT_BATTERY);
    setRobotPos(environment->getStationPos());
}

Robot::Robot(Environment *environment, string name, int batteryCapacity)
{
    _name = name;
    _battery = new Battery(batteryCapacity);
    _environment = environment;
    setRobotPos(environment->getStationPos());
}

Robot::Robot(Environment *environment, string fileName)
{
    _environment = environment;
    string line; //auxiliary variable to read each line of the robotFile
    ifstream robotFile; //auxiliary variable to open the robotFile
    string battery_str; //string created to receive the battery charge string from file

    robotFile.open (fileName);

    // Check if the file is open
    if(robotFile.is_open())
    {
        while(getline(robotFile, line))
        {
            // push each line to the list _fileLines
            _fileLines.push_back(line);
        }
        robotFile.close();
    } else
    {
        cout<<"ERROR! << Robot file >>"<<endl;
    }

    // Get each line of the file, where the first one has the robot name and the second has the robot battery charge
    list<string>::iterator it = _fileLines.begin();
    _name = *it;
    it++;
    battery_str = *it;
    _batteryCapacity = stoi(battery_str);
    _battery = new Battery(_batteryCapacity);
    setRobotPos(environment->getStationPos());
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


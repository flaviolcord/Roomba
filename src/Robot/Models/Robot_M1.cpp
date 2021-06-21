#include "Robot_M1.h"

#define DEFAULT_BATTERY 100
#define UNCLEAN 0
#define OBSTACLE 1
#define STATION 2
#define CLEANED 3

// Todos os parametros que Robot pode receber Robot_M1 deve poder receber tbm!
Robot_M1::Robot_M1(Environment *environment) : Robot(environment)
{
    _environment = environment;
    _bumper = new Bumper(environment);
    _batteryCharge = DEFAULT_BATTERY;
}

Robot_M1::Robot_M1(Environment *environment, string name, float batteryCharge) : Robot(environment, name, batteryCharge)
{
    _environment = environment;
    _bumper = new Bumper(environment);
    _batteryCharge = batteryCharge;
}

void Robot_M1::moveRobot()
{
    list<Position> neighborsFree;
    int sign = 1;
    Position robotPos = getRobotPos();
    Position pos1, pos2;

    // Find free positions
    for(int i = 0; i < 2; i++)
    {
        if(i == 1) { sign = -1;}
        pos1 = Position(true, robotPos.x() + sign, robotPos.y());
        pos2 = Position(true, robotPos.x(), robotPos.y() + sign);

        if(_environment->getPosValue(pos1) != OBSTACLE && _environment->getPosValue(pos1) != STATION)
        {
            neighborsFree.push_back(pos1);
        }
        if(_environment->getPosValue(pos2) != OBSTACLE && _environment->getPosValue(pos2) != STATION)
        {
            neighborsFree.push_back(pos2);
        }
    }

    // Define position to move
    if(neighborsFree.size() > 1)
    {
        // sortear position
        list<Position>::iterator it = neighborsFree.begin();
        int idPos = rand() %neighborsFree.size() + 0;

        // checkar
        advance(it, idPos);
        setRobotPos(*it);

    } else if (neighborsFree.size() == 1)
    {
        setRobotPos(neighborsFree.front());
    } else {
        // ERROR
    }
}

void Robot_M1::clean()
{
    while (this->getBatteryValue() > 0.2*_batteryCharge)
    {
        moveRobot();

        //Check if the cell is clean
        if(_environment->getPosValue(getRobotPos()) == UNCLEAN)
        {
            // Clean
            // retira uma unidade
            updateBattery(0, false);
            _environment->setGridValue(getRobotPos(), CLEANED);

        }
    }

}


#include "Robot_M1.h"

#define DEFAULT_BATTERY 100
#define UNCLEAN 0
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

    // Discharge the battery one unit of charge
    updateBattery(0, false);
    // Find free positions
    for(int i = 0; i < 2; i++)
    {
        if(i == 1) { sign = -1;}

        pos1 = Position(true, robotPos.x() + sign, robotPos.y());
        pos2 = Position(true, robotPos.x(), robotPos.y() + sign);

        // check positions pos1 and pos2
        checkPos(pos1);
        checkPos(pos2);

        if(!_bumper->checkObstacle(pos1) && pos1 != _environment->getStationPos() && !pos1.isInvalid())
        {
            neighborsFree.push_back(pos1);
        }
        if(!_bumper->checkObstacle(pos2) && pos2 != _environment->getStationPos() && !pos2.isInvalid())
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

        advance(it, idPos);
        setRobotPos(*it);

    } else if (neighborsFree.size() == 1)
    {
        setRobotPos(neighborsFree.front());
    } else {
        // Error
        cout<<"ERROR! Robot blocked!"<<endl;
    }
}

void Robot_M1::clean()
{
    // check if _environment = nullptr
    Position robotPos;
    if(_environment == nullptr)
    {
        cout<<"ERROR! Environment ptr is null <<Robot_M1>>"<<endl;
        return ;
    }
    while (getBatteryValue() > 2)
    {
        moveRobot();

        //Check if the cell is clean

        if(_environment->getPosValue(getRobotPos()) == UNCLEAN)
        {
            // Clean
            // retira uma unidade
            updateBattery(0, false);
            _environment->setPosValue(getRobotPos(), CLEANED);

        }
    }

    _environment->setPosValue(getRobotPos(), 5);

}

void Robot_M1::checkPos(Position &pos)
{
    if(pos.x() < 0 || pos.x() > _environment->getDimensionX())
    {
        pos.setInvalid();
    }

    if(pos.y() < 0 || pos.y() > _environment->getDimensionY())
    {
        pos.setInvalid();
    }
}

// Incompleta - nao verifica obstaculos
void Robot_M1::returnRobotToStation()
{
    Position robotPos, stationPos;
    int deltaX, deltaY;

    while (getRobotPos() != _environment->getStationPos() && getBatteryValue() > 0)
    {
        robotPos = getRobotPos();
        stationPos = _environment->getStationPos();

        // Calculate deltaX e deltaY
        deltaX = abs(robotPos.x() - stationPos.x());
        deltaY = abs(robotPos.y() - stationPos.y());

        if(deltaX > 0)
        {
            if(robotPos.x() > stationPos.x())
            {
                robotPos = Position(true, robotPos.x() - 1, robotPos.y());

            } else {
                robotPos = Position(true, robotPos.x() + 1, robotPos.y());
            }
        }
        if(deltaY > 0)
        {
            if(robotPos.y() > stationPos.y())
            {
                robotPos = Position(true, robotPos.x(), robotPos.y() - 1);

            } else {
                robotPos = Position(true, robotPos.x(), robotPos.y() + 1);
            }
        }
        setRobotPos(robotPos);
    }
}


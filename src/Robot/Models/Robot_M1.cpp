#include "Robot_M1.h"

#define DEFAULT_BATTERY 200
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
    if(neighborsFree.size() >= 1)
    {
        // sort position
        list<Position>::iterator it = neighborsFree.begin();
        int idPos = rand() %(neighborsFree.size()-1) + 0;

        advance(it, idPos);
        setRobotPos(*it);

    } else {
        // Error
        cout<<"ERROR! Robot blocked!"<<endl;
    }
}



void Robot_M1::clean()
{
    // check if _environment = nullptr
    Position robotPos;
    Position pos;
    int it(0);
    if(_environment == nullptr)
    {
        cout<<"ERROR! Environment ptr is null <<Robot_M1>>"<<endl;
        return ;
    }
    int _cellUnclean=0;
    for (int i=0;i<_environment->getDimensionY();i++)
    {
        for (int j=0;j<_environment->getDimensionX();j++)
        {
            pos.setPosition(true, j, i);
            if (_environment->getPosValue(pos)==0)
                _cellUnclean++;
        }
    }

    while (_cellUnclean>0 && it<600)
    {
        while (getBatteryValue() > 0.2*_batteryCharge)
        {
            moveRobot();

            //Check if the cell is clean

            if(_environment->getPosValue(getRobotPos()) == UNCLEAN)
            {
                // Clean
                // retira uma unidade
                updateBattery(0, false);
                _cellUnclean--; //reduce the number of uncleaned cells
                _environment->setPosValue(getRobotPos(), CLEANED);
                cout << getRobotPos().y() << ", " << getRobotPos().x() << endl;
            }
        }
        returnRobotToStation();
        it++;
    }
    cout << _cellUnclean << endl;
}

void Robot_M1::checkPos(Position &pos)
{
    if(pos.x() < 0 || pos.x() > (_environment->getDimensionX()-1))
    {
        pos.setInvalid();
    }

    if(pos.y() < 0 || pos.y() > (_environment->getDimensionY()-1))
    {
        pos.setInvalid();
    }
}

void Robot_M1::returnRobotToStation()
{
    Position robotPos, stationPos;
    int deltaX, deltaY;
    int sign_dX, sign_dY;
    int move_rob;
    Position pos1, pos2;
    stationPos = _environment->getStationPos();

    //cout << "in: " << getRobotPos().x() << "," << getRobotPos().y() << endl;
    while ((robotPos != stationPos) && (getBatteryValue() > 0))
    {
        //robotPos = getRobotPos();
        move_rob=0;
        // Calculate deltaX e deltaY
        deltaX = (stationPos.x() - robotPos.x());
        deltaY = (stationPos.y() - robotPos.y());

        sign_dX = -1 * (deltaX < 0) + 1 * (deltaX > 0);
        sign_dY = -1 * (deltaY < 0) + 1 * (deltaY > 0);

        pos1 = Position(true, robotPos.x() + (sign_dX), robotPos.y());
        checkPos(pos1);
        if(!_bumper->checkObstacle(pos1) && !pos1.isInvalid() && deltaX!=0)
        {
            robotPos = pos1;
            move_rob=1;
        }
        else
        {
            pos2 = Position(true, robotPos.x(), robotPos.y() + sign_dY);
            checkPos(pos2);
            if(!_bumper->checkObstacle(pos2) && !pos2.isInvalid() && deltaY!=0)
            {
                robotPos = pos2;
                move_rob=1;
            }
            else
            {
                pos2 = Position(true, robotPos.x(), robotPos.y() - sign_dY);
                checkPos(pos2);
                if(!_bumper->checkObstacle(pos2) && !pos2.isInvalid() && deltaY!=0)
                {
                    robotPos = pos2;
                    move_rob = 1;
                }
            }
        }
        if (move_rob==0)
        {
            pos1 = Position(true, robotPos.x() - (sign_dX), robotPos.y());
            checkPos(pos1);
            if(!_bumper->checkObstacle(pos1) && !pos1.isInvalid() && deltaX!=0)
            {
                robotPos = pos1;
            }
        }
        updateBattery(0, false);
    }
    setRobotPos(robotPos);
    if (getRobotPos() == _environment->getStationPos())
    {
        updateBattery(_batteryCharge - getBatteryValue(), 1);
        _environment->setPosValue(getRobotPos(), 5);
    }
}


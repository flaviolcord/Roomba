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
    _batteryCapacity = DEFAULT_BATTERY;
}

Robot_M1::Robot_M1(Environment *environment, string name, int batteryCharge) : Robot(environment, name, batteryCharge)
{
    _environment = environment;
    _bumper = new Bumper(environment);
    _batteryCapacity = batteryCharge;
}

Robot_M1::Robot_M1(Environment *environment, string file_name) : Robot(environment, file_name)
{
    _environment = environment;
    _bumper = new Bumper(environment);
    _batteryCapacity = getBatteryValue();
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

    Position robotPos;
    Position pos;
    int it(0); //variable to compute how many times the robot has returned to the station
    int _cellUnclean = 0; //variable to compute the number of cells to be cleaned

    // check if _environment = nullptr
    if(_environment == nullptr)
    {
        cout << "ERROR! Environment ptr is null <<Robot_M1>>"<<endl;
        return ;
    }

    //compute the number of cells to be cleaned
    for (int i=0;i<_environment->getDimensionY();i++)
    {
        for (int j=0;j<_environment->getDimensionX();j++)
        {
            pos.setPosition(true, j, i);
            if (_environment->getPosValue(pos)==0)
                _cellUnclean++;
        }
    }

    // while there is uncleaned cells and the robot had already returned to the station 600 times, make it move to clean
    while (_cellUnclean>0 && it<600)
    {
        while (getBatteryValue() > 0.2*_batteryCapacity)
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
            }
        }

        returnRobotToStation();
        if (getBatteryValue()==0)
        {
            cout << "Could not go back to station" << endl;
            return;
        }

        it++;
    }
    cout << "The number of remaining unclean cells is: " << _cellUnclean << endl;
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
    Position robotPos, stationPos, robotPos_old;
    int deltaX, deltaY;
    int sign_dX, sign_dY;
    int move_rob;
    Position pos1, pos2, pos3, pos4;
    stationPos = _environment->getStationPos();
    robotPos = getRobotPos();
    robotPos_old = robotPos;

    while ((robotPos != stationPos) && (getBatteryValue() > 0))
    {

        move_rob = 0;

        // Calculate deltaX e deltaY
        deltaX = (stationPos.x() - robotPos.x());
        deltaY = (stationPos.y() - robotPos.y());

        sign_dX = -1 * (deltaX < 0) + 1 * (deltaX > 0);
        sign_dY = -1 * (deltaY < 0) + 1 * (deltaY > 0);

        if (deltaX==0 && robotPos.y()==robotPos_old.y())
        {
            sign_dX = 1;
        }
        if (deltaY==0 && robotPos.x()==robotPos_old.x())
        {
            sign_dY = 1;
        }
        pos1 = Position(true, robotPos.x() + (sign_dX), robotPos.y());
        checkPos(pos1);
        if(!_bumper->checkObstacle(pos1) && !pos1.isInvalid() && deltaX!=0 && pos1.x()!=robotPos_old.x())
        {
            robotPos = pos1;
            move_rob=1;
        }
        else
        {
            pos2 = Position(true, robotPos.x(), robotPos.y() + sign_dY && pos2.y()!=robotPos_old.y());
            checkPos(pos2);
            if(!_bumper->checkObstacle(pos2) && !pos2.isInvalid())
            {
                robotPos = pos2;
                move_rob=1;
            }
            else
            {
                pos3 = Position(true, robotPos.x(), robotPos.y() - sign_dY);
                checkPos(pos3);
                if(!_bumper->checkObstacle(pos3) && !pos3.isInvalid() && pos3.y()!=robotPos_old.y())
                {
                    robotPos = pos3;
                    move_rob = 1;
                }
            }
        }
        if (move_rob==0)
        {
            pos4 = Position(true, robotPos.x() - (sign_dX), robotPos.y());
            checkPos(pos4);
            if(!_bumper->checkObstacle(pos4) && !pos4.isInvalid() && pos4.y()!=robotPos_old.y())
            {
                robotPos = pos4;
                move_rob = 1;
            }
        }
        if (move_rob==0){
            cout<< "Robot is trapped." << endl;
            break;}
        else{
            updateBattery(0, false);
        }
        robotPos_old = robotPos;
    }
    setRobotPos(robotPos);
    if (getRobotPos() == stationPos)
    {
        updateBattery(_batteryCapacity - getBatteryValue(), 1);
        _environment->setPosValue(getRobotPos(), 5);
    }


}


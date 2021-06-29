#include "Robot_M1.h"
#include <cmath>

#define DEFAULT_BATTERY 200
#define INTERACTIONS_LIMIT 50
#define UNCLEAN 0
#define OBSTACLE 1
#define STATION 2
#define CLEANED 3
#define ROBOT_STATION 5


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
    int sign = 1;
    list<Position> neighborsFree;
    Position robotPos = getRobotPos();
    Position pos1, pos2;

    // Discharge the battery one unit of charge
    updateBattery(0, false);

    // Find free positions for the four directions
    for(int i = 0; i < 2; i++)
    {
        if(i == 1) { sign = -1;}

        pos1 = Position(true, robotPos.x() + sign, robotPos.y());
        pos2 = Position(true, robotPos.x(), robotPos.y() + sign);

        // Check positions pos1 and pos2 are valid positions
        checkPos(pos1);
        checkPos(pos2);

        // If each position is not an obstacle, the station position or a invalid position
        // this position is saved in the list "neighborsFree"
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
    // If neighborsFree.size() > 1 the position is chosen randomly
    if(neighborsFree.size() >= 1)
    {
        // sort position
        list<Position>::iterator it = neighborsFree.begin();

        int idPos = _environment->generate_random(0, (neighborsFree.size()-1));
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

    // check if _environment == nullptr
    if(_environment == nullptr)
    {
        cout << "ERROR! Environment ptr is null <<Robot_M1>>"<<endl;
        return ;
    }

    //compute the number of cells to be cleaned
    int _cellUnclean = _environment->get_clean_cells(); //variable to compute the number of cells to be cleaned

    /* while there is uncleaned cells and the robot had already returned to the station a number of times
    that is less than the limit defined by INTERACTIONS_LIMIT, it will move to clean */
    while (_cellUnclean>0 && it < INTERACTIONS_LIMIT)
    {
        while (getBatteryValue() > 0.2*_batteryCapacity)
        {
            // Move robot to the next cell
            moveRobot();

            //Check if the cell is clean
            if(_environment->getPosValue(getRobotPos()) == UNCLEAN)
            {
                // Clean
                // Discharge the battery one unit of charge
                updateBattery(0, false);
                _cellUnclean--; //reduce the number of uncleaned cells
                _environment->setPosValue(getRobotPos(), CLEANED); // set cell to CLEANED identifier
            }
        }

        returnRobotToStation(); //return robot to station
        // if the battery is discharged, than the robot could not go back to the station in time
        if (batteryDischarged())
        {
            cout << "Could not go back to station." << endl;
            return;
        }
        it++; //update iteration variable counter
    }
    cout<<"\n\n";
    cout<<"The number of recharges is: "<<it<<endl;
    cout << "The number of remaining unclean cells is: " << _cellUnclean << endl;
}

// Check if the robot position is out of matrix
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

    //initialization of 'old' variables with any bigger value than deltaX and deltaY can have, so it passes the initial loop
    int deltaX_old(10000), deltaY_old(10000);
    int sign_dX, sign_dY;
    int move_rob; //variable to check if the robot has already moved (it is only allowed one move per iteration)
    Position pos; //variable to detect robot new position
    stationPos = _environment->getStationPos(); // get station position in grid
    robotPos = getRobotPos(); // get robot current position
    bool trap; //auxiliary variable to identify traps
    //counter of the while loop for getting the robot movement (if counter_try > 4, the robot could not move - is trapped)
    int counter_try;

    // the robot will move (try to return to the station) while it's not already there and there is still battery
    while ((robotPos != stationPos) && !batteryDischarged())
    {

        move_rob = 0; //initialize variable at every iteration
        counter_try = 0; //initialize variable at every iteration

        // Calculate robot distance to station by means of the x and y axis (movement axis)
        deltaX = (stationPos.x() - robotPos.x());
        deltaY = (stationPos.y() - robotPos.y());

        // if deltaX is negative, the closest that the robot will get to the station is if it decrements its X position
        // if delts X is positive, the robot have to increment its X position
        // ** THE SAME WRITTEN ABOVE OCCURS FOR deltaY:
        sign_dX = -1 * (deltaX < 0) + 1 * (deltaX > 0);
        sign_dY = -1 * (deltaY < 0) + 1 * (deltaY > 0);

        /* However, deltaX==0 and the robot cannot move closer to the station in Y, the code will
        try to move the robot in the X direction (this is considered inside the while loop).
        When it moves further in X, (abs(deltaX_old)<abs(deltaX)) is true, so the code understands that
        the next movement cannot be in X towards the station, because it will go back to the known trap.
        In this case, it will try to move in the Y direction again, to get closer to the station,
        but if a trap is seen, it will move in the same X direction as before, further from the station.
        This procedure is analogous for the case where deltaY == zero and the robot is trapped in X.
        Every explanation given here is written bellow in code lines.
        */
        if (abs(deltaX_old)<abs(deltaX))
        {
            sign_dX = 0;
        }
        if (abs(deltaY_old)<abs(deltaY))
        {
            sign_dY = 0;
        }

        while ((move_rob==0) && counter_try<=4)
        {
            trap = true;
            // Try first move in X accordingly with the sign_dX computed that takes the robot closer to the station
            pos = Position(true, robotPos.x() + (sign_dX), robotPos.y());
            checkPos(pos);
            // check if its a valid position
            if(!_bumper->checkObstacle(pos) && !pos.isInvalid() && sign_dX!=0)
            {
                move_rob=1;
            }
            else
            {
                pos = Position(true, robotPos.x(), robotPos.y() + sign_dY);
                checkPos(pos);
                // check if its a valid position
                if(!_bumper->checkObstacle(pos) && !pos.isInvalid() && sign_dY!=0)
                {
                    move_rob=1;
                }
                else if (deltaX==0){sign_dX=1; trap=false;}
                    else{sign_dX=-sign_dX; trap=false;}
            }
            /* if the robot could not move in none of the directions above, then try to move it in the opposite Y direction
            which is further from the station or in some Y direction if deltaY==0 (which means the trap is in the X direction*/
            if (move_rob==0 && trap)
            {
                if (deltaY==0){sign_dY=1;}
                else sign_dY=-sign_dY;
            }
            counter_try++;
        }
        if (move_rob==0)
        {
            cout << "The robot is trapped in all its directions of movement." << endl;
            break;
        }

        // update robot old position
        deltaY_old = deltaY;
        deltaX_old = deltaX;
        // move robot
        robotPos = pos;
        // if the robot has moved, update battery level
        updateBattery(0, false);
    }

    // set robot new position
    setRobotPos(robotPos);

    // if the robot is actually in the station: charge battery and set its position to the Station Identifier
    if (getRobotPos() == stationPos)
    {
        updateBattery(_batteryCapacity - getBatteryValue(), true);
        _environment->setPosValue(getRobotPos(), ROBOT_STATION);
    }
}


#include "Robot_M2.h"
#include <math.h>

#define DEFAULT_BATTERY 200
#define INTERACTIONS_LIMIT 50
#define UNCLEAN 0
#define OBSTACLE 1
#define STATION 2
#define CLEANED 3
#define ROBOT_STATION 5
#define DEFAULT_ORIENTATION 0

Robot_M2::Robot_M2(Environment *environment) : Robot(environment)
{
    _environment = environment;
    _laser = new Laser(_environment);
    _batteryCapacity = DEFAULT_BATTERY;
    _orientation = DEFAULT_ORIENTATION;
}

Robot_M2::Robot_M2(Environment *environment, string name, int batteryCharge) : Robot(environment, name, batteryCharge)
{
    _environment = environment;
    _laser = new Laser(_environment);
    _batteryCapacity = batteryCharge;
    _orientation = DEFAULT_ORIENTATION;
}

Robot_M2::Robot_M2(Environment *environment, string file_name) : Robot(environment, file_name)
{
    _environment = environment;
    _laser = new Laser(_environment);
    _batteryCapacity = getBatteryValue();
    _orientation = DEFAULT_ORIENTATION;
}

void Robot_M2::moveRobot()
{
    list<Position> neighborsFree;
    Position robotPos = getRobotPos();
    Position startPos, pos;
    float old_orientation;

    // Discharge the battery one unit of charge for movement
    updateBattery(0, false);

    startPos = Position(true, robotPos.x()-1, robotPos.y()-1);
    // Find free positions for the eight directions
    // i - line / j - columns
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {

            pos = Position(true, startPos.x()+j, startPos.y()+i);

            // Check if is a valid position
            checkPos(pos);

            // If "pos" is not an obstacle, the robot position, the station position or a invalid position
            // this position is saved in the list "neighborsFree"
            if(pos != _environment->getStationPos() && pos != robotPos && !pos.isInvalid())
            {
                neighborsFree.push_back(pos);
            }
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
        old_orientation = _orientation;
        turn_robot(robotPos, *it);
        // Discharge the battery one unit of charge for turning
        if (old_orientation!=_orientation){updateBattery(0, false);}
        setRobotPos(*it);


    } else {
        // Error
        cout<<"ERROR! Robot blocked!"<<endl;
    }
}

void Robot_M2::clean()
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
                // Discharge the battery one unit of charge for cleaning
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
void Robot_M2::checkPos(Position &pos)
{
    if(pos.x() < 0 || pos.x() > (_environment->getDimensionX()-1))
    {
        pos.setInvalid();
    }
     else if(pos.y() < 0 || pos.y() > (_environment->getDimensionY()-1))
    {
        pos.setInvalid();
    }
    else if(_laser->checkObstacle(pos))
    {
        pos.setInvalid();
    }
}

void Robot_M2::turn_robot(Position old_pos, Position new_pos)
{
    int deltaX = new_pos.x() - old_pos.x();
    int deltaY = new_pos.y() - old_pos.y();

    if (deltaX>0)
    {
        if (deltaY<0){_orientation=M_PI/4;}
        else if (deltaY>0){_orientation=7*M_PI/4;}
        else if (deltaY==0){_orientation=0;}
    }
    else if (deltaX<0)
    {
        if (deltaY<0){_orientation=3*M_PI/4;}
        else if (deltaY>0){_orientation=5*M_PI/4;}
        else if (deltaY==0){_orientation=M_PI;}
    }
    else
    {
        if (deltaY<0){_orientation=M_PI/2;}
        else if (deltaY>0){_orientation=3*M_PI/2;}
    }
    //_orientation = M_PI*((1/4) + (dX<0)*(dY<0) + 2*(dX<0)*(dY>0)+ 3*(dX>0)*(dY>0) + ...
    //... (dX==0) * (1/4 *(dY<0) + 5/4 * (dY>0)) -1/4*(dY==0)*(dX<0))
}

void Robot_M2::returnRobotToStation()
{
    Position robotPos, stationPos;
    int deltaX, deltaY;
    //initialization of 'old' variables with any bigger value than deltaX and deltaY can have, so it passes the initial loop
    int deltaX_old(10000), deltaY_old(10000);
    int sign_dX, sign_dY;
    float old_orientation; //to find out if the robot is turning and, therefore, spending battery
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
        // if deltaX is positive, the robot have to increment its X position
        // ** THE SAME WRITTEN ABOVE OCCURS FOR deltaY:
        sign_dX = -1 * (deltaX < 0) + 1 * (deltaX > 0);
        sign_dY = -1 * (deltaY < 0) + 1 * (deltaY > 0);

        /* Explaining the "if" statement written bellow (and also, part of the while loop):
        If deltaX==0 and the robot cannot move closer to the station in Y, the code will
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

        // Try first move in the direction that takes the robot closer to the station
        pos = Position(true, robotPos.x() + sign_dX, robotPos.y() + sign_dY);
        checkPos(pos);
        // check if its a valid position
        if(!_laser->checkObstacle(pos) && !pos.isInvalid())
        {
            move_rob=1;
        }
        while ((move_rob==0) && counter_try<=4)
        {
            trap = true;
            // Try first move in X accordingly with the sign_dX computed that takes the robot closer to the station
            pos = Position(true, robotPos.x() + (sign_dX), robotPos.y());
            checkPos(pos);
            // check if its a valid position
            if(!_laser->checkObstacle(pos) && !pos.isInvalid() && sign_dX!=0)
            {
                move_rob=1;
            }
            else
            {
                // if it's not valid, try to move the robot in the Y direction, closer to the station
                pos = Position(true, robotPos.x(), robotPos.y() + sign_dY);
                checkPos(pos);
                // check if its a valid position
                if(!_laser->checkObstacle(pos) && !pos.isInvalid() && sign_dY!=0)
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

        //turn robot
        old_orientation = _orientation;
        turn_robot(robotPos_old,pos);

        // Discharge the battery one unit of charge for turning
        if (old_orientation!=_orientation){updateBattery(0, false);}

        // move robot
        robotPos = pos;

        // Discharge the battery one unit of charge for movement
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


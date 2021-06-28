#include "Robot_M2.h"

#define DEFAULT_BATTERY 200
#define INTERACTIONS_LIMIT 50
#define UNCLEAN 0
#define OBSTACLE 1
#define STATION 2
#define CLEANED 3
#define ROBOT_STATION 5

Robot_M2::Robot_M2(Environment *environment) : Robot(environment)
{
    _environment = environment;
    _laser = new Laser(_environment);
    _batteryCapacity = DEFAULT_BATTERY;
}

Robot_M2::Robot_M2(Environment *environment, string name, int batteryCharge) : Robot(environment, name, batteryCharge)
{
    _environment = environment;
    _laser = new Laser(_environment);
    _batteryCapacity = batteryCharge;
}

Robot_M2::Robot_M2(Environment *environment, string file_name) : Robot(environment, file_name)
{
    _environment = environment;
    _laser = new Laser(_environment);
    _batteryCapacity = getBatteryValue();
}

void Robot_M2::moveRobot()
{
    list<Position> neighborsFree;
    Position robotPos = getRobotPos();
    Position startPos, pos;

    // Discharge the battery one unit of charge
    updateBattery(0, false);

    startPos = Position(true, robotPos.x()-1, robotPos.y()-1);
    // Find free positions for the four directions
    // i - line / j - collumns
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
                // Discharge the battery one unit of charge
                updateBattery(0, false);
                _cellUnclean--; //reduce the number of uncleaned cells
                _environment->setPosValue(getRobotPos(), CLEANED); // set cell to CLEANED identifier
            }
        }
        return;
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


void Robot_M2::returnRobotToStation()
{
    Position robotPos, stationPos, robotPos_old;
    int deltaX, deltaY;
    int sign_dX, sign_dY;
    int move_rob; //variable to check if the robot has already moved (it is only allowed one move per iteration)
    Position pos1, pos2, pos3, pos4; //robot 4 possible positions of movement
    stationPos = _environment->getStationPos(); // get station position in grid
    robotPos = getRobotPos(); // get robot current position
    robotPos_old = robotPos; // variable to get robot old position

    // the robot will move (try to return to the station) while it's not already there and there is still battery
    while ((robotPos != stationPos) && !batteryDischarged())
    {

        move_rob = 0; //initialize variable at every iteration

        // Calculate robot distance to station by means of the x and y axis (movement axis)
        deltaX = (stationPos.x() - robotPos.x());
        deltaY = (stationPos.y() - robotPos.y());

        // if deltaX is negative, the closest that the robot will get to the station is if it decrements its X position
        // if delts X is positive, the robot have to increment its X position
        // ** THE SAME WRITTEN ABOVE OCCURS FOR deltaY:
        sign_dX = -1 * (deltaX < 0) + 1 * (deltaX > 0);
        sign_dY = -1 * (deltaY < 0) + 1 * (deltaY > 0);

        /* However, if the robot is already align in the X position with the station and the robot Y position
         of the current iteration is equal of the Y position of the last iteration, this means that the
         robot is trapped, so in order to get out of the trap, it has to move in the X direction. We here
         assumed that it will move in the positive direction of X, but if this is not a valid move,
         then it will try the negative direction of X. This procedure is analogous for the case where
         deltaY is zero and the robot is trapped in X.
        */
        if (deltaX==0 && robotPos.y()==robotPos_old.y())
        {
            sign_dX = 1;
        }
        if (deltaY==0 && robotPos.x()==robotPos_old.x())
        {
            sign_dY = 1;
        }
        // update robot old position
        robotPos_old = robotPos;

        // Try first move in X accordingly with the sign_dX computed that takes the robot closer to the station
        pos1 = Position(true, robotPos.x() + (sign_dX), robotPos.y());
        checkPos(pos1);
        // check if its a valid position
        if(!_laser->checkObstacle(pos1) && !pos1.isInvalid() && deltaX!=0 && pos1.x()!=robotPos_old.x())
        {
            // move robot
            robotPos = pos1;
            move_rob=1;
        }
        else
        {
            // if it's not valid, try to move the robot in the Y direction, closer to the station
            pos2 = Position(true, robotPos.x(), robotPos.y() + sign_dY && pos2.y()!=robotPos_old.y());
            checkPos(pos2);
            // check if its a valid position
            if(!_laser->checkObstacle(pos2) && !pos2.isInvalid())
            {
                // move robot
                robotPos = pos2;
                move_rob=1;
            }
            else
            {
                /* if it's not valid, than the robot is seeing a trap when trying to move closer to
                the station. So, try to move the robot in the opposite Y direction,
                further from the station */
                pos3 = Position(true, robotPos.x(), robotPos.y() - sign_dY);
                checkPos(pos3);
                if(!_laser->checkObstacle(pos3) && !pos3.isInvalid() && pos3.y()!=robotPos_old.y())
                {
                    //move robot
                    robotPos = pos3;
                    move_rob = 1;
                }
            }
        }
        /* if the robot could not move in none of the directions above, then move it in the opposite X direction
        which is further from the station*/
        if (move_rob==0)
        {
            pos4 = Position(true, robotPos.x() - (sign_dX), robotPos.y());
            checkPos(pos4);
            if(!_laser->checkObstacle(pos4) && !pos4.isInvalid() && pos4.y()!=robotPos_old.y())
            {
                robotPos = pos4;
                move_rob = 1;
            }
        }
        // if the robot still could'nt  move, than it means he is trapped in all of its directions of movement
        if (move_rob==0){
            // show an error message
            cout<< "Robot is trapped in all its directions of movement." << endl;
            // break the loop
            break;}
        else{
            // if the robot has moved, update battery level
            updateBattery(0, false);
        }


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


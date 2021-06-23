// --------------------------------------------------------------------

//  Rommba

//  UFPE - ENGENHARIA MECANICA
//  Disciplina: Programacao Avancada
//  Professor: Adrien Joan Sylvain Durand Petiteville
//  Dupla: Eduarda Galindo e Flavio Cordeiro

// ---------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>

#include "src/Environment/Environment.h"
#include "src/Robot/Models/Robot_M1.h"
#include "src/Types/Position.h"

using namespace std;

int main ()
{
    int option;
    string fileName;
    int x;
    int y;
    int obstacle_option;
    int robot_data_opt;
    int battery_capacity;
    string obstacle_fileName;
    string robot_name;
    string robot_file_name;

    cout << "Choose one of the options bellow:" << endl;
    cout << "1 - import grid from test.txt file" << endl;
    cout << "2 - create new grid" << endl;
    cout << "Enter here your option (1 or 2): ";
    cin >> option;

    Environment *test;

    if (option==1)
    {
        fileName = "teste.txt";
        test = new Environment(fileName);
    }
    if (option==2)
    {
        cout << "Enter here grid dimensions (enter an integer value): " << endl;
        cout << "x: ";
        cin >> x;
        cout << "y: ";
        cin >> y;
        test = new Environment(x, y);
        cout << "Add obstacle to your mesh. Choose: " << endl;
        cout << "1 - Create obstacle cell. " << endl;
        cout << "2 - Create obstacle shaped as an rectangle. " << endl;
        cout << "3 - Import obstacle coordinates from teste_obstacle.txt file. " << endl;
        cout << "Enter here your option: ";
        cin >> obstacle_option;

        if (obstacle_option==1)
        {
            test->addObstacle(false);
        }
        else if (obstacle_option==2)
        {
            test->addObstacle(true);
        }
        else if (obstacle_option==3)
        {
            obstacle_fileName = "teste_obstacles.txt";
            test->addObstacle(obstacle_fileName);
        }
    }

    test->printGrid();
    cout<<"\n\n";
    Robot_M1 *robot;

    cout << "Choose one of the options bellow in order to insert robot into the grid: " << endl;
    cout << "1 - Write robot data" << endl;
    cout << "2 - Import robot data from file 'robot.txt'" << endl;
    cout << "If you want to use the default data, enter any number different from 1 and 2"<< endl;
    cout << "Enter here your option: ";
    cin >> robot_data_opt;

    if (robot_data_opt==1)
    {
        cout << "Enter robot name: ";
        getline(cin,robot_name);
        getline(cin,robot_name); //only one its not working
        cout << "Enter robot battery capacity: ";
        cin >> battery_capacity;
        robot= new Robot_M1(test, robot_name, battery_capacity);

    }
    else if (robot_data_opt==2)
    {
        robot_file_name = "robot.txt";
        robot = new Robot_M1(test, robot_file_name);
    }
    else
    {
        robot = new Robot_M1(test);
    }
    robot->clean();
    test->printGrid();
    return 0;
}

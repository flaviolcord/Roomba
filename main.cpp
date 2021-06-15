#include <iostream>
#include <fstream>
#include <string>

#include "src/Environment/Environment.h"

using namespace std;

int main ()
{
    int option;
    string fileName;
    int x;
    int y;
    int obstacle_option;
    string obstacle_fileName;

    cout << "Choose one of the options bellow:" << endl;
    cout << "1 - import grid from test.txt file" << endl;
    cout << "2 - create new grid" << endl;
    cout << "Enter here your option (1 or 2): ";
    cin >> option;

    if (option==1) //it would be better if this was a function called by main
    {
        fileName = "test.txt";
        Environment *test = new Environment(fileName);
        test->printGrid();
    }
    if (option==2) //it would be better if this was a function called by main
    {
        cout << "Enter here grid dimensions (enter an integer value): " << endl;
        cout << "x: ";
        cin >> x;
        cout << "y: ";
        cin >> y;
        Environment *test = new Environment(x, y);
        cout << "Add obstacle to your mesh. Choose: " << endl;
        cout << "1 - Create obstacle cell. " << endl;
        cout << "2 - Create obstacle shaped as an rectangle. " << endl;
        cout << "3 - Import obstacle coordinates from texte_obstacle.txt file. " << endl;
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
        test->printGrid();
    }


    return 0;
}

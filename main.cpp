#include <iostream>
#include <fstream>
#include <string>

#include <src/Environment/Environment.h>

using namespace std;

int main () {

    string fileName;
    fileName = "test.txt";

    Environment *test = new Environment(fileName);

    test->printGrid();
    return 0;
}

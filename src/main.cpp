#include "student-menu.h"
#include "input-utils.h"
#include "custom-vector.h"

#include <deque>
#include <iostream>
#include <list>

int main() {
    cout << "Pasirinkite konteinerio tipa:" << endl;
    cout << "1 - vector<Student>" << endl;
    cout << "2 - list<Student>" << endl;
    cout << "3 - deque<Student>" << endl;
    cout << "4 - CustomVector<Student>" << endl;

    const int containerChoice = readIntInRange("Pasirinkimas: ", 1, 4);

    switch (containerChoice) {
        case 1:
            startMainMenu<vector<Student>>();
            break;
        case 2:
            startMainMenu<std::list<Student>>();
            break;
        case 3:
            startMainMenu<std::deque<Student>>();
            break;
        case 4:
            startMainMenu<CustomVector<Student>>();
            break;
    }

    return 0;
}
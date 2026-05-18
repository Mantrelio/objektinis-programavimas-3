#include "sort-students.h"

#include "custom-vector.h"
#include "input-utils.h"

#include <algorithm>
#include <iostream>
#include <deque>
#include <list>
#include <type_traits>

using std::cout;
using std::endl;
using std::sort;

template <typename T>
void sortStudents(T& students, int sortChoice, bool ascending) {
    if (sortChoice == 0) {
        return;
    }

    auto lessByChoice = [sortChoice](const Student& a, const Student& b) {
        switch (sortChoice) {
            case 1:
                if (a.name() == b.name()) return a.surname() < b.surname();
                return a.name() < b.name();
            case 2:
                if (a.surname() == b.surname()) return a.name() < b.name();
                return a.surname() < b.surname();
            case 3:
                return a.finalGradeAverage() < b.finalGradeAverage();
            case 4:
                return a.finalGradeMedian() < b.finalGradeMedian();
            default:
                return a.name() < b.name();
        }
    };

    if (ascending) {
        if constexpr (std::is_same_v<T, std::list<Student>>) {
            students.sort(lessByChoice);
        } else {
            sort(students.begin(), students.end(), lessByChoice);
        }
    } else {
        if constexpr (std::is_same_v<T, std::list<Student>>) {
            students.sort([&lessByChoice](const Student& a, const Student& b) {
                return lessByChoice(b, a);
            });
        } else {
            sort(students.begin(), students.end(),
                [&lessByChoice](const Student& a, const Student& b) {
                    return lessByChoice(b, a);
                }
            );
        }
    }
}
template void sortStudents(vector<Student>&, int, bool);
template void sortStudents(std::list<Student>&, int, bool);
template void sortStudents(std::deque<Student>&, int, bool);
template void sortStudents(CustomVector<Student>&, int, bool);

template <typename T>
void runSortStudentsChoicePrompt(T& students) {
    int sortChoice;
    cout << "\nPasirinkite rūšiavimo būdą:" << endl;
    cout << "0 - Nerūšiuotas (pradinė tvarka)" << endl;
    cout << "1 - Pagal vardą" << endl;
    cout << "2 - Pagal pavardę" << endl;
    cout << "3 - Pagal galitinį vidurkį" << endl;
    cout << "4 - Pagal galitinę medianą" << endl;
    sortChoice = readIntInRange("Pasirinkimas: ", 0, 4);

    if (sortChoice == 0) {
        cout << "Rodomi nerūšiuoti rezultatai (pradinėje tvarkoj)." << endl;
        return;
    }

    cout << "\nPasirinkite rūšiavimo tvarką:" << endl;
    cout << "1 - Didėjanti" << endl;
    cout << "2 - Mažėjanti" << endl;
    int orderChoice = readIntInRange("Pasirinkimas: ", 1, 2);

    sortStudents(students, sortChoice, orderChoice == 1);
}
template void runSortStudentsChoicePrompt(vector<Student>&);
template void runSortStudentsChoicePrompt(std::list<Student>&);
template void runSortStudentsChoicePrompt(std::deque<Student>&);
template void runSortStudentsChoicePrompt(CustomVector<Student>&);
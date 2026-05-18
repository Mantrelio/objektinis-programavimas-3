#include "output-nuskriausti-and-protingi-test.h"
#include "custom-vector.h"
#include "input-utils.h"
#include "create-student.h"
#include "sort-students.h"
#include "sort-nuskriausti-and-protingi.h"

#include <chrono>
#include <deque>
#include <iomanip>
#include <iostream>
#include <list>
#include <vector>

using std::cout;
using std::endl;

template <typename Container>
void outputNuskriaustiAndProtingiTestForContainer(const string& filename, int strategyChoice) {
    const int sortChoice = 3;
    const bool ascending = true;

    double totalLoadSeconds = 0.0;
    double totalSortSeconds = 0.0;
    double totalSplitSeconds = 0.0;
    double totalIterationSeconds = 0.0;

    for (int i = 0; i < 5; i++) {
        const auto loadStart = std::chrono::steady_clock::now();
        Container students = createStudentsFromFile<Container>(filename);
        const auto loadEnd = std::chrono::steady_clock::now();

        const auto sortStart = std::chrono::steady_clock::now();
        sortStudents(students, sortChoice, ascending);
        const auto sortEnd = std::chrono::steady_clock::now();

        const auto splitStart = std::chrono::steady_clock::now();

        switch(strategyChoice) {
            case 1:
                sortNuskriaustiAndProtingiFirstStrategy(students);
                break;
            case 2:
                sortNuskriaustiAndProtingiSecondStrategy(students);
                break;
            case 3:
                sortNuskriaustiAndProtingiThirdStrategy(students);
                break;
        }

        const auto splitEnd = std::chrono::steady_clock::now();

        const double loadSeconds = std::chrono::duration<double>(loadEnd - loadStart).count();
        const double sortSeconds = std::chrono::duration<double>(sortEnd - sortStart).count();
        const double splitSeconds = std::chrono::duration<double>(splitEnd - splitStart).count();
        const double iterationSeconds = loadSeconds + sortSeconds + splitSeconds;

        totalLoadSeconds += loadSeconds;
        totalSortSeconds += sortSeconds;
        totalSplitSeconds += splitSeconds;
        totalIterationSeconds += iterationSeconds;
    }

    const double averageLoadSeconds = totalLoadSeconds / 5.0;
    const double averageSortSeconds = totalSortSeconds / 5.0;
    const double averageSplitSeconds = totalSplitSeconds / 5.0;
    const double averageIterationSeconds = totalIterationSeconds / 5.0;

    cout << std::fixed << std::setprecision(6)
         << "\nVidurkiai per 5 iteracijas:\n"
         << "  Failo nuskaitymas: " << averageLoadSeconds << " s\n"
         << "  Rusiavimas: " << averageSortSeconds << " s\n"
         << "  Skirstymas i du konteinerius: " << averageSplitSeconds << " s\n"
         << "  Is viso: " << averageIterationSeconds << " s" << endl;
}

void outputNuskriaustiAndProtingiTestForAllStudentsTxtFiles() {
    const vector<string> files = {"students1000", "students10000", "students100000", "students1000000", "students10000000"};

    cout << "\nPasirinkite konteinerio tipa:" << endl;
    cout << "1 - vector<Student>" << endl;
    cout << "2 - list<Student>" << endl;
    cout << "3 - deque<Student>" << endl;
    cout << "4 - CustomVector<Student>" << endl;

    const int containerChoice = readIntInRange("Pasirinkimas: ", 1, 4);

    cout << "\nPasirinkite strategija:" << endl;
    cout << "1 - Pirma" << endl;
    cout << "2 - Antra" << endl;
    cout << "3 - Trecia" << endl;

    const int strategyChoice = readIntInRange("Pasirinkimas: ", 1, 3);

    for (const string& filename : files) {
        cout << "\n=== Testuojamas failas: " << filename << " ===" << endl;

        switch (containerChoice) {
            case 1:
                outputNuskriaustiAndProtingiTestForContainer<vector<Student>>(filename, strategyChoice);
                break;
            case 2:
                outputNuskriaustiAndProtingiTestForContainer<std::list<Student>>(filename, strategyChoice);
                break;
            case 3:
                outputNuskriaustiAndProtingiTestForContainer<std::deque<Student>>(filename, strategyChoice);
                break;
            case 4:
                outputNuskriaustiAndProtingiTestForContainer<CustomVector<Student>>(filename, strategyChoice);
                break;
        }
    }
}

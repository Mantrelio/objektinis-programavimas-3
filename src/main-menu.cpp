#include "student-menu.h"

#include "custom-vector.h"
#include "create-student.h"
#include "input-utils.h"
#include "output-results.h"
#include "generate-student-list.h"
#include "generate-student-list-test.h"
#include "output-nuskriausti-and-protingi-test.h"
#include "student-rule-of-five-operators-test.h"
#include "custom-vector-push-back-test.h"
#include "custom-vector-reallocation-test.h"
#include "sort-nuskriausti-and-protingi.h"
#include "sort-students.h"

#include <iostream>
#include <deque>
#include <list>
#include <stdexcept>

namespace {

void announceNextStudent(int studentIndex) {
    cout << "\n--- Studentas " << studentIndex << " ---" << endl;
}

}  // namespace

void showMainMenu() {
    cout << "\n=== MENIU ===" << endl;
    cout << "1 - Rankinė įvestis (įvesti visas reikšmes)" << endl;
    cout << "2 - Įvesti vardą/pavardę, generuoti pažymius" << endl;
    cout << "3 - Generuoti visus duomenis atsitiktinai" << endl;
    cout << "4 - Skaityti studentus iš failo" << endl;
    cout << "5 - Generuoti studentu sarasa" << endl;
    cout << "6 - Skirstyti i nuskriaustus ir protingus" << endl;
    cout << "7 - Testuoti failo kurima" << endl;
    cout << "8 - Testuoti studentu skirstyma i nuskriaustus ir protingus" << endl;
    cout << "9 - Testuoti Student Rule of Five ir operatorius" << endl;
    cout << "10 - Palyginti CustomVector ir std::vector push_back" << endl;
    cout << "11 - Palyginti CustomVector ir std::vector atminties perskirstymus" << endl;
    cout << "12 - Baigti ir peržiūrėti rezultatus" << endl;
}

template <typename T>
void handleMenuChoice(int choice, T& students) {
    int nextIndex = students.size() + 1;

    switch (choice) {
        case 1:
            announceNextStudent(nextIndex);
            students.push_back(createStudentManual());
            break;
        case 2:
            announceNextStudent(nextIndex);
            students.push_back(createStudentRandomGrades());
            break;
        case 3:
            announceNextStudent(nextIndex);
            students.push_back(createStudentFullyRandom());
            break;
        case 4: {
            const string filename = readSingleStringToken("Suveskite failo pavadinimą (pvz. data.txt): ");
            T fileStudents;
            try {
                fileStudents = createStudentsFromFile<T>(filename);
                cout << "Sėkmingai nuskaityti " << fileStudents.size() << " studentai iš failo." << endl;
            } catch (const std::exception& e) {
                cout << "Klaida skaitant \"" << filename << "\": " << e.what() << endl;
                break;
            }
            students.insert(students.end(), fileStudents.begin(), fileStudents.end());
            break;
        }
        case 5: 
            handleGenerateStudentListFile();
            break;
        case 6: {
            handleSortNuskriaustiAndProtingiFromFile<T>();
            break;
        }
        case 7: 
            handleGenerateStudentListTest();
            break;
        case 8: 
            outputNuskriaustiAndProtingiTestForAllStudentsTxtFiles();
            break;
        case 9:
            handleStudentRuleOfFiveAndOperatorsTest();
            break;
        case 10:
            handleCustomVectorPushBackTest();
            break;
        case 11:
            handleCustomVectorReallocationTest();
            break;
        case 12:
            runSortStudentsChoicePrompt(students);
            chooseOutputAndPrint(students);
            break;
    }
}

template void handleMenuChoice<vector<Student>>(int, vector<Student>&);
template void handleMenuChoice<std::list<Student>>(int, std::list<Student>&);
template void handleMenuChoice<std::deque<Student>>(int, std::deque<Student>&);
template void handleMenuChoice<CustomVector<Student>>(int, CustomVector<Student>&);

template <typename T>
void startMainMenu() {
    T students;

    int choice = 0;

    while (choice != 12) {
        showMainMenu();
        choice = readIntInRange("Pasirinkimas: ", 1, 12);
        handleMenuChoice(choice, students);
    }
}

template void startMainMenu<vector<Student>>();
template void startMainMenu<std::list<Student>>();
template void startMainMenu<std::deque<Student>>();
template void startMainMenu<CustomVector<Student>>();

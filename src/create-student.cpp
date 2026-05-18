#include "create-student.h"

#include "custom-vector.h"

#include "input-utils.h"
#include "grading-utils.h"

#include <deque>
#include <fstream>
#include <list>
#include <sstream>
#include <utility>

using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::move;
using std::string;
using std::vector;

Student createStudentManual() {
    const string name = readSingleStringToken("Suveskite studento vardą: ");
    const string surname = readSingleStringToken("Suveskite studento pavard: ");
    const int examGrade = readIntInRange("Suveskite egzamino pažymį: ", 1, 10);
    vector<int> homeworkGrades;

    char continueHomework = 'Y';

    while (continueHomework == 'Y') {
        const int grade = readIntInRange("Suveskite namu darbo pažymį: ", 1, 10);
        homeworkGrades.push_back(grade);

        continueHomework = readYesOrNo("Pridėti dar vieną namu darbo pažymį? (y/n): ");
    }

    return Student(name, surname, homeworkGrades, examGrade);
}

Student createStudentRandomGrades() {
    const string name = readSingleStringToken("Suveskite studento vardą: ");
    const string surname = readSingleStringToken("Suveskite studento pavardę: ");
    const int examGrade = randomGrade();
    vector<int> homeworkGrades;

    cout << "Sugeneruotas egzamino pažymys: " << examGrade << endl;

    const int homeworkCount = randomGrade(3, 10);
    homeworkGrades.reserve(homeworkCount);

    cout << "Sugeneruoti " << homeworkCount << " namu darbo pažymiai: ";
    for (int i = 0; i < homeworkCount; i++) {
        const int grade = randomGrade();
        homeworkGrades.push_back(grade);
        cout << grade;
        if (i < homeworkCount - 1) {
            cout << ", ";
        }
    }
    cout << endl;

    return Student(name, surname, homeworkGrades, examGrade);
}

string randomName() {
    const string names[] = {"Jonas", "Petras", "Antanas", "Marija", "Ona", "Jurate", "Tomas", "Mindaugas", "Ruta", "Greta"};
    return names[rand() % 10];
}

string randomSurname() {
    const string surnames[] = {"Jonaitis", "Petraitis", "Antanaitis", "Kazlauskas", "Zukauskas", "Jankauskas", "Paulauskas", "Stankevicius", "Vasiliauskas", "Baranauskas"};
    return surnames[rand() % 10];
}

Student createStudentFullyRandom() {
    const string name = randomName();
    const string surname = randomSurname();
    const int examGrade = randomGrade();

    const int homeworkCount = randomGrade(3, 10);
    vector<int> homeworkGrades;
    homeworkGrades.reserve(homeworkCount);

    for (int i = 0; i < homeworkCount; i++) {
        homeworkGrades.push_back(randomGrade());
    }

    return Student(name, surname, homeworkGrades, examGrade);
}

template <typename T>
T createStudentsFromFile(const string& filename) {
    T studentsFromFile;
    ifstream in(filename);

    if (!in.is_open()) {
        throw std::runtime_error("Nepavyko atidaryti failo");
    }

    string headerLine;
    getline(in, headerLine);

    string line;
    int lineNumber = 1;

    while (getline(in, line)) {
        ++lineNumber;

        if (line.empty()) {
            continue;
        }

        istringstream iss(line);
        Student student;
        iss >> student;

        if (iss.fail()) {
            cout << "Praleistos eilutės " << lineNumber
                 << ": invalid student record format" << endl;
            continue;
        }

        studentsFromFile.push_back(move(student));
    }

    return studentsFromFile;
}

template vector<Student> createStudentsFromFile(const string& filename);
template std::list<Student> createStudentsFromFile(const string& filename);
template std::deque<Student> createStudentsFromFile(const string& filename);
template CustomVector<Student> createStudentsFromFile(const string& filename);

#include "output-results.h"

#include "custom-vector.h"
#include "input-utils.h"

#include <deque>
#include <fstream>
#include <iomanip>
#include <list>
#include <stdexcept>

using std::endl;
using std::left;
using std::ofstream;
using std::right;
using std::runtime_error;
using std::setw;

template <typename T>
void outputResults(const T& students, std::ostream& out) {
    if (students.empty()) {
        cout << "Studentų sąrašas tuščias." << endl;
        return;
    }

    out << "\n" << string(70, '=') << endl;
    out << left << setw(20) << "Vardas"
        << left << setw(20) << "Pavardė"
        << right << setw(15) << "Gal. (Vid.)"
        << right << setw(15) << "Gal. (Med.)" << endl;
    out << string(70, '-') << endl;

    for (const Student& student : students) {
        out << student << endl;
    }
    out << string(70, '=') << endl;
}

template void outputResults(const vector<Student>& students, std::ostream& out);
template void outputResults(const std::list<Student>& students, std::ostream& out);
template void outputResults(const std::deque<Student>& students, std::ostream& out);
template void outputResults(const CustomVector<Student>& students, std::ostream& out);

template <typename T>
void chooseOutputAndPrint(T& students) {
    cout << "\nPasirinkite išvedimo būdą:" << endl;
    cout << "1 - Rodyti rezultatus konsoleje" << endl;
    cout << "2 - Išsaugoti rezultatus į tekstinį failą" << endl;

    const int outputChoice = readIntInRange("Pasirinkimas: ", 1, 2);

    if (outputChoice == 2) {
        const string outFilename = readSingleStringToken("Suveskite išsaugų failo pavadinimą (pvz. results.txt): ");
        try {
            ofstream outFile(outFilename);
            if (!outFile) {
                throw runtime_error("failed to open output file for writing");
            }

            outputResults(students, outFile);
            outFile.flush();
        } catch (const std::exception& e) {
            cout << "Nepavyko išsaugoti rezultatų į \"" << outFilename << "\": " << e.what() << endl;
        }
        return;
    }

    outputResults(students, cout);
}

template void chooseOutputAndPrint(vector<Student>& students);
template void chooseOutputAndPrint(std::list<Student>& students);
template void chooseOutputAndPrint(std::deque<Student>& students);
template void chooseOutputAndPrint(CustomVector<Student>& students);
#include "sort-nuskriausti-and-protingi.h"

#include "custom-vector.h"

#include <exception>
#include <iostream>

#include <algorithm>
#include <deque>
#include <list>

template <typename T>
std::pair<T, T> sortNuskriaustiAndProtingiFirstStrategy(const T& students) {
	T nuskriausti;
	T protingi;

	for (const Student& student : students) {
		if (student.finalGradeAverage() >= 5) {
			protingi.push_back(student);
		} else {
			nuskriausti.push_back(student);
		}
	}

	return {nuskriausti, protingi};
}

template std::pair<vector<Student>, vector<Student>> sortNuskriaustiAndProtingiFirstStrategy(const vector<Student>&);
template std::pair<std::list<Student>, std::list<Student>> sortNuskriaustiAndProtingiFirstStrategy(const std::list<Student>&);
template std::pair<std::deque<Student>, std::deque<Student>> sortNuskriaustiAndProtingiFirstStrategy(const std::deque<Student>&);
template std::pair<CustomVector<Student>, CustomVector<Student>> sortNuskriaustiAndProtingiFirstStrategy(const CustomVector<Student>&);

template <typename T>
T sortNuskriaustiAndProtingiSecondStrategy(T& students) {
	T nuskriausti;

	for (const Student& student : students) {
		if (student.finalGradeAverage() < 5) {
			nuskriausti.push_back(student);
		}
	}

	students.erase(
		std::remove_if(students.begin(), students.end(), [](const Student& student) {
			return student.finalGradeAverage() < 5;
		}),
		students.end()
	);

	return nuskriausti;
}

template vector<Student> sortNuskriaustiAndProtingiSecondStrategy(vector<Student>&);
template std::list<Student> sortNuskriaustiAndProtingiSecondStrategy(std::list<Student>&);
template std::deque<Student> sortNuskriaustiAndProtingiSecondStrategy(std::deque<Student>&);
template CustomVector<Student> sortNuskriaustiAndProtingiSecondStrategy(CustomVector<Student>&);

template <typename T>
std::pair<T, T> sortNuskriaustiAndProtingiThirdStrategy(T& students) {
	auto splitPoint = std::stable_partition(students.begin(), students.end(), [](const Student& student) {
		return student.finalGradeAverage() >= 5;
	});

	T protingi;
	T nuskriausti;

	protingi.insert(protingi.end(), students.begin(), splitPoint);
	nuskriausti.insert(nuskriausti.end(), splitPoint, students.end());

	return {nuskriausti, protingi};
}

template std::pair<vector<Student>, vector<Student>> sortNuskriaustiAndProtingiThirdStrategy(vector<Student>&);
template std::pair<std::list<Student>, std::list<Student>> sortNuskriaustiAndProtingiThirdStrategy(std::list<Student>&);
template std::pair<std::deque<Student>, std::deque<Student>> sortNuskriaustiAndProtingiThirdStrategy(std::deque<Student>&);
template std::pair<CustomVector<Student>, CustomVector<Student>> sortNuskriaustiAndProtingiThirdStrategy(CustomVector<Student>&);

template <typename T>
void handleSortNuskriaustiAndProtingiFromFile() {
	const string filename = readSingleStringToken("Suveskite failo pavadinimą (pvz. data.txt): ");

	try {
		T fileStudents = createStudentsFromFile<T>(filename);

		runSortStudentsChoicePrompt(fileStudents);

		cout << "\nPasirinkite strategija:" << endl;
		cout << "1 - Pirma" << endl;
		cout << "2 - Antra" << endl;
		cout << "3 - Trecia" << endl;

		const int strategyChoice = readIntInRange("Pasirinkimas: ", 1, 3);

		T nuskriausti;
		T protingi;

		switch (strategyChoice) {
			case 1: {
				auto splitResult = sortNuskriaustiAndProtingiFirstStrategy(fileStudents);
				nuskriausti = std::move(splitResult.first);
				protingi = std::move(splitResult.second);
				break;
			}
			case 2:
				nuskriausti = sortNuskriaustiAndProtingiSecondStrategy(fileStudents);
				protingi = std::move(fileStudents);
				break;
			case 3: {
				auto splitResult = sortNuskriaustiAndProtingiThirdStrategy(fileStudents);
				nuskriausti = std::move(splitResult.first);
				protingi = std::move(splitResult.second);
				break;
			}
		}

		cout << "\n--- Nuskriausti ---" << endl;
		chooseOutputAndPrint(nuskriausti);

		cout << "\n--- Protingi ---" << endl;
		chooseOutputAndPrint(protingi);
	} catch (const std::exception& e) {
		cout << "Klaida skaitant \"" << filename << "\": " << e.what() << endl;
	}
}

template void handleSortNuskriaustiAndProtingiFromFile<vector<Student>>();
template void handleSortNuskriaustiAndProtingiFromFile<std::list<Student>>();
template void handleSortNuskriaustiAndProtingiFromFile<std::deque<Student>>();
template void handleSortNuskriaustiAndProtingiFromFile<CustomVector<Student>>();
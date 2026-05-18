#include "custom-vector-push-back-test.h"

#include "custom-vector.h"
#include "input-utils.h"

#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <vector>

static const int kIterations = 1;

static std::size_t readElementCount() {
    std::cout << "Pasirinkite elementu skaiciu:" << std::endl;
    std::cout << "1 - 10000" << std::endl;
    std::cout << "2 - 100000" << std::endl;
    std::cout << "3 - 1000000" << std::endl;
    std::cout << "4 - 10000000" << std::endl;
    std::cout << "5 - 100000000" << std::endl;

    const int choice = readIntInRange("Pasirinkite varianta: ", 1, 5);

    switch (choice) {
        case 1:
            return 10000;
        case 2:
            return 100000;
        case 3:
            return 1000000;
        case 4:
            return 10000000;
        case 5:
            return 100000000;
        default:
            return 10000;
    }
}

template <typename Vector>
static double measurePushBack(std::size_t count) {
    Vector vector;

    const auto start = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < count; ++i) {
        vector.push_back(static_cast<typename Vector::value_type>(i));
    }
    const auto end = std::chrono::steady_clock::now();

    if (vector.size() != count) {
        std::cout << "Klaida: push_back neuzpilde konteinerio." << std::endl;
    }

    return std::chrono::duration<double>(end - start).count();
}

template <typename Vector>
static void runPushBackBenchmark(const char* label, std::size_t count) {
    double totalSeconds = 0.0;

    std::cout << "\n--- " << label << " ---" << std::endl;

    for (int i = 0; i < kIterations; ++i) {
        const double iterationSeconds = measurePushBack<Vector>(count);
        totalSeconds += iterationSeconds;

        std::cout << std::fixed << std::setprecision(6) << "  Iteracija " << (i + 1)
                  << ": " << iterationSeconds << " s" << std::endl;
    }

    const double averageSeconds = totalSeconds / static_cast<double>(kIterations);
    std::cout << std::fixed << std::setprecision(6) << "  Vidurkis: " << averageSeconds
              << " s" << std::endl;
}

void handleCustomVectorPushBackTest() {
    const std::size_t count = readElementCount();

    std::cout << "\n=== push_back našumo palyginimas (int) ===" << std::endl;
    std::cout << "Elementu skaicius: " << count << std::endl;
    std::cout << "Iteraciju skaicius: " << kIterations << std::endl;

    runPushBackBenchmark<CustomVector<int>>("CustomVector<int>::push_back", count);
    runPushBackBenchmark<std::vector<int>>("std::vector<int>::push_back", count);
}

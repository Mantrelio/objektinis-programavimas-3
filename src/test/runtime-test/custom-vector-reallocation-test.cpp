#include "custom-vector-reallocation-test.h"

#include "custom-vector.h"

#include <cstddef>
#include <iostream>
#include <vector>

constexpr std::size_t kElementCount = 100'000'000;

template <typename Vector>
std::size_t countReallocationsWhilePushBack(std::size_t element_count) {
    Vector vector;
    std::size_t reallocations = 0;

    for (std::size_t i = 0; i < element_count; ++i) {
        if (vector.size() == vector.capacity()) {
            ++reallocations;
        }
        vector.push_back(static_cast<typename Vector::value_type>(i));
    }

    return reallocations;
}

void handleCustomVectorReallocationTest() {
    std::cout << "\n=== Atminties perskirstymu palyginimas (int) ===" << std::endl;
    std::cout << "Elementu skaicius: " << kElementCount << std::endl;

    std::cout << "\nCustomVector<int>..." << std::endl;
    const std::size_t customReallocations =
        countReallocationsWhilePushBack<CustomVector<int>>(kElementCount);
    std::cout << "  Perskirstymu: " << customReallocations << std::endl;

    std::cout << "\nstd::vector<int>..." << std::endl;
    const std::size_t standardReallocations =
        countReallocationsWhilePushBack<std::vector<int>>(kElementCount);
    std::cout << "  Perskirstymu: " << standardReallocations << std::endl;

    std::cout << "\n--- Rezultatas ---" << std::endl;
    std::cout << "CustomVector: " << customReallocations << ", std::vector: "
              << standardReallocations << std::endl;
}

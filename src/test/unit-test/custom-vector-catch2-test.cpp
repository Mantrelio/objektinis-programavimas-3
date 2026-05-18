#include "catch_amalgamated.hpp"

#include "custom-vector.h"

#include <cstddef>
#include <limits>
#include <list>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

template <typename T>
CustomVector<T> make_vector(std::initializer_list<T> values) {
    CustomVector<T> vector;
    for (const T& value : values) {
        vector.push_back(value);
    }
    return vector;
}

}  // namespace

TEST_CASE("CustomVector default constructor creates empty vector", "[custom-vector][constructors]") {
    CustomVector<int> vector;

    REQUIRE(vector.size() == 0);
    REQUIRE(vector.capacity() == 0);
    REQUIRE(vector.empty());
    REQUIRE(vector.data() == nullptr);
    REQUIRE(vector.begin() == vector.end());
    REQUIRE(vector.cbegin() == vector.cend());
}

TEST_CASE("CustomVector copy constructor performs deep copy", "[custom-vector][constructors]") {
    const CustomVector<int> original = make_vector({1, 2, 3});

    const CustomVector<int> copy(original);

    REQUIRE(copy == original);
    REQUIRE(copy.size() == 3);
    REQUIRE(copy.capacity() == original.capacity());

    const_cast<CustomVector<int>&>(original).push_back(4);
    REQUIRE(copy != original);
    REQUIRE(copy.size() == 3);
}

TEST_CASE("CustomVector move constructor transfers ownership", "[custom-vector][constructors]") {
    CustomVector<int> source = make_vector({5, 6, 7});
    const auto source_capacity = source.capacity();

    CustomVector<int> moved(std::move(source));

    REQUIRE(moved == make_vector({5, 6, 7}));
    REQUIRE(moved.capacity() == source_capacity);
    REQUIRE(source.size() == 0);
    REQUIRE(source.capacity() == 0);
    REQUIRE(source.empty());
}

TEST_CASE("CustomVector copy assignment performs deep copy", "[custom-vector][assignment]") {
    const CustomVector<int> source = make_vector({10, 20});
    CustomVector<int> target = make_vector({99});

    target = source;

    REQUIRE(target == source);
    REQUIRE(target.size() == 2);

    const_cast<CustomVector<int>&>(source).push_back(30);
    REQUIRE(target != source);
}

TEST_CASE("CustomVector copy assignment handles self-assignment", "[custom-vector][assignment]") {
    CustomVector<int> vector = make_vector({1, 2, 3});

    vector = vector;

    REQUIRE(vector == make_vector({1, 2, 3}));
}

TEST_CASE("CustomVector move assignment transfers ownership", "[custom-vector][assignment]") {
    CustomVector<int> source = make_vector({7, 8, 9});
    CustomVector<int> target = make_vector({0});

    target = std::move(source);

    REQUIRE(target == make_vector({7, 8, 9}));
    REQUIRE(source.size() == 0);
    REQUIRE(source.capacity() == 0);
}

TEST_CASE("CustomVector swap exchanges contents", "[custom-vector][swap]") {
    CustomVector<int> left = make_vector({1, 2});
    CustomVector<int> right = make_vector({3, 4, 5});
    const auto left_capacity = left.capacity();
    const auto right_capacity = right.capacity();

    left.swap(right);

    REQUIRE(left == make_vector({3, 4, 5}));
    REQUIRE(right == make_vector({1, 2}));
    REQUIRE(left.capacity() == right_capacity);
    REQUIRE(right.capacity() == left_capacity);
}

TEST_CASE("CustomVector assign replaces contents", "[custom-vector][assign]") {
    CustomVector<int> vector = make_vector({1, 2, 3});

    vector.assign(4, 42);

    REQUIRE(vector == make_vector({42, 42, 42, 42}));
    REQUIRE(vector.size() == 4);
    REQUIRE(vector.capacity() == 4);

    vector.assign(0, 0);
    REQUIRE(vector.empty());
    REQUIRE(vector.capacity() == 0);
}

TEST_CASE("CustomVector insert inserts at valid positions", "[custom-vector][insert]") {
    CustomVector<int> vector = make_vector({1, 3});

    const int* const at_begin = vector.insert(vector.begin(), 0);
    REQUIRE(*at_begin == 0);
    REQUIRE(vector == make_vector({0, 1, 3}));

    const int* const in_middle = vector.insert(vector.begin() + 2, 2);
    REQUIRE(*in_middle == 2);
    REQUIRE(vector == make_vector({0, 1, 2, 3}));

    const int* const at_end = vector.insert(vector.end(), 4);
    REQUIRE(*at_end == 4);
    REQUIRE(vector == make_vector({0, 1, 2, 3, 4}));
}

TEST_CASE("CustomVector insert throws on invalid position", "[custom-vector][insert]") {
    CustomVector<int> vector = make_vector({1});

    REQUIRE_THROWS_AS(vector.insert(vector.begin() - 1, 0), std::out_of_range);
    REQUIRE_THROWS_AS(vector.insert(vector.end() + 1, 0), std::out_of_range);
}

TEST_CASE("CustomVector insert_range inserts multiple elements", "[custom-vector][insert]") {
    CustomVector<int> vector = make_vector({1, 5});
    const std::vector<int> middle{2, 3, 4};

    const int* const inserted_at = vector.insert_range(vector.begin() + 1, middle.begin(), middle.end());

    REQUIRE(inserted_at == vector.begin() + 1);
    REQUIRE(vector == make_vector({1, 2, 3, 4, 5}));

    const int* const unchanged = vector.insert_range(vector.end(), middle.begin(), middle.begin());
    REQUIRE(unchanged == vector.end());
    REQUIRE(vector == make_vector({1, 2, 3, 4, 5}));
}

TEST_CASE("CustomVector insert_range throws on invalid position", "[custom-vector][insert]") {
    CustomVector<int> vector = make_vector({1});
    const std::vector<int> values{2};

    REQUIRE_THROWS_AS(vector.insert_range(vector.end() + 1, values.begin(), values.end()),
                     std::out_of_range);
}

TEST_CASE("CustomVector emplace constructs element in place", "[custom-vector][emplace]") {
    CustomVector<std::string> vector;
    vector.push_back("end");

    const std::string* const inserted = vector.emplace(vector.begin(), 3, 'x');

    REQUIRE(*inserted == "xxx");
    REQUIRE(vector.size() == 2);
    REQUIRE(vector.front() == "xxx");
    REQUIRE(vector.back() == "end");
}

TEST_CASE("CustomVector emplace throws on invalid position", "[custom-vector][emplace]") {
    CustomVector<std::string> vector;

    REQUIRE_THROWS_AS(vector.emplace(vector.end() + 1, 1, 'a'), std::out_of_range);
}

TEST_CASE("CustomVector push_back and emplace_back append elements", "[custom-vector][modifiers]") {
    CustomVector<std::string> vector;

    vector.push_back("a");
    vector.emplace_back("b");

    REQUIRE(vector == make_vector<std::string>({"a", "b"}));
    REQUIRE(vector.back() == "b");
}

TEST_CASE("CustomVector pop_back removes last element", "[custom-vector][modifiers]") {
    CustomVector<int> vector = make_vector({1, 2, 3});

    vector.pop_back();

    REQUIRE(vector == make_vector({1, 2}));
    REQUIRE(vector.back() == 2);
}

TEST_CASE("CustomVector append_range appends elements at end", "[custom-vector][modifiers]") {
    CustomVector<int> vector = make_vector({1});
    const std::list<int> tail{2, 3};

    vector.append_range(tail.begin(), tail.end());

    REQUIRE(vector == make_vector({1, 2, 3}));
}

TEST_CASE("CustomVector erase removes single element", "[custom-vector][erase]") {
    CustomVector<int> vector = make_vector({1, 2, 3});

    const int* const after_erase = vector.erase(vector.begin() + 1);

    REQUIRE(after_erase == vector.begin() + 1);
    REQUIRE(vector == make_vector({1, 3}));
}

TEST_CASE("CustomVector erase removes element range", "[custom-vector][erase]") {
    CustomVector<int> vector = make_vector({1, 2, 3, 4, 5});

    const int* const after_erase = vector.erase(vector.begin() + 1, vector.begin() + 4);

    REQUIRE(after_erase == vector.begin() + 1);
    REQUIRE(vector == make_vector({1, 5}));
}

TEST_CASE("CustomVector erase returns first on empty range", "[custom-vector][erase]") {
    CustomVector<int> vector = make_vector({1, 2, 3});

    const int* const unchanged = vector.erase(vector.begin() + 1, vector.begin() + 1);

    REQUIRE(unchanged == vector.begin() + 1);
    REQUIRE(vector == make_vector({1, 2, 3}));
}

TEST_CASE("CustomVector erase throws on invalid range", "[custom-vector][erase]") {
    CustomVector<int> vector = make_vector({1, 2});

    REQUIRE_THROWS_AS(vector.erase(vector.end()), std::out_of_range);
    REQUIRE_THROWS_AS(vector.erase(vector.begin() - 1), std::out_of_range);
    REQUIRE_THROWS_AS(vector.erase(vector.end(), vector.begin()), std::out_of_range);
}

TEST_CASE("CustomVector element access provides references", "[custom-vector][access]") {
    CustomVector<int> vector = make_vector({10, 20, 30});

    REQUIRE(vector.at(1) == 20);
    REQUIRE(vector[2] == 30);
    REQUIRE(vector.front() == 10);
    REQUIRE(vector.back() == 30);

    vector.at(0) = 11;
    vector[1] = 21;
    vector.front() = 12;
    vector.back() = 32;

    REQUIRE(vector == make_vector({12, 21, 32}));
}

TEST_CASE("CustomVector at throws on out of range index", "[custom-vector][access]") {
    CustomVector<int> vector = make_vector({1});

    REQUIRE_THROWS_AS(vector.at(1), std::out_of_range);
}

TEST_CASE("CustomVector capacity and size track elements", "[custom-vector][capacity]") {
    CustomVector<int> vector;

    REQUIRE(vector.max_size() > 0);
    REQUIRE(vector.max_size() <= std::numeric_limits<std::size_t>::max());

    vector.reserve(8);
    REQUIRE(vector.capacity() >= 8);
    REQUIRE(vector.size() == 0);
    REQUIRE(vector.empty());

    vector.push_back(1);
    vector.push_back(2);
    REQUIRE(vector.size() == 2);
    REQUIRE_FALSE(vector.empty());
}

TEST_CASE("CustomVector reserve grows capacity and preserves elements", "[custom-vector][capacity]") {
    CustomVector<int> vector = make_vector({1, 2, 3});
    const auto old_capacity = vector.capacity();

    vector.reserve(old_capacity + 5);

    REQUIRE(vector.capacity() >= old_capacity + 5);
    REQUIRE(vector == make_vector({1, 2, 3}));

    vector.reserve(old_capacity);
    REQUIRE(vector.capacity() >= old_capacity + 5);
}

TEST_CASE("CustomVector resize grows and shrinks", "[custom-vector][capacity]") {
    CustomVector<int> vector = make_vector({1, 2, 3});

    vector.resize(5, 9);
    REQUIRE(vector == make_vector({1, 2, 3, 9, 9}));

    vector.resize(2, 0);
    REQUIRE(vector == make_vector({1, 2}));
}

TEST_CASE("CustomVector resize throws when count exceeds max_size", "[custom-vector][capacity]") {
    CustomVector<int> vector;

    REQUIRE_THROWS_AS(vector.resize(vector.max_size() + 1, 0), std::length_error);
}

TEST_CASE("CustomVector shrink_to_fit reduces capacity to size", "[custom-vector][capacity]") {
    CustomVector<int> vector = make_vector({1, 2, 3});
    vector.reserve(16);
    REQUIRE(vector.capacity() >= 16);

    vector.shrink_to_fit();
    REQUIRE(vector.capacity() == vector.size());
    REQUIRE(vector == make_vector({1, 2, 3}));

    vector.clear();
    vector.shrink_to_fit();
    REQUIRE(vector.empty());
    REQUIRE(vector.capacity() == 0);
}

TEST_CASE("CustomVector clear removes all elements", "[custom-vector][capacity]") {
    CustomVector<int> vector = make_vector({1, 2, 3});

    vector.clear();

    REQUIRE(vector.empty());
    REQUIRE(vector.size() == 0);
    REQUIRE(vector.begin() == vector.end());
}

TEST_CASE("CustomVector iterators and data expose element storage", "[custom-vector][iterators]") {
    CustomVector<int> vector = make_vector({4, 5, 6});

    REQUIRE(vector.data() == vector.begin());
    REQUIRE(static_cast<std::size_t>(vector.end() - vector.begin()) == vector.size());
    REQUIRE(static_cast<std::size_t>(vector.cend() - vector.cbegin()) == vector.size());

    REQUIRE(*vector.rbegin() == 6);
    REQUIRE(*vector.crbegin() == 6);
    REQUIRE(vector.rend() == std::reverse_iterator<int*>(vector.begin()));
    REQUIRE(vector.crend() == std::reverse_iterator<const int*>(vector.cbegin()));
}

TEST_CASE("CustomVector comparison operators compare lexicographically", "[custom-vector][compare]") {
    const CustomVector<int> smaller = make_vector({1, 2});
    const CustomVector<int> larger = make_vector({1, 3});
    const CustomVector<int> equal = make_vector({1, 2});
    const CustomVector<int> prefix = make_vector({1});

    REQUIRE(smaller == equal);
    REQUIRE(smaller != larger);
    REQUIRE(smaller < larger);
    REQUIRE(larger > smaller);
    REQUIRE(smaller <= equal);
    REQUIRE(larger >= smaller);
    REQUIRE(prefix < smaller);
    REQUIRE(smaller >= prefix);
}

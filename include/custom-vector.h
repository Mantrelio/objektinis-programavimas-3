#pragma once

#include <cstddef>
#include <memory>
#include <utility>

template <typename T>
class CustomVector {
public:
    using value_type = T;
    using size_type = std::size_t;

    CustomVector() noexcept;
    CustomVector(const CustomVector& other);
    CustomVector(CustomVector&& other) noexcept;
    CustomVector& operator=(const CustomVector& other);
    CustomVector& operator=(CustomVector&& other) noexcept;
    ~CustomVector() = default;

    void swap(CustomVector& other) noexcept;

    void assign(size_type count, const T& value);

private:
    void reset() noexcept;

    std::unique_ptr<T[]> data_;
    size_type size_;
    size_type capacity_;
};

#include "custom-vector.tpp"

#pragma once

#include <cstddef>
#include <iterator>
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
    T* insert(T* pos, const T& value);

    template <typename InputIt>
    T* insert_range(T* pos, InputIt first, InputIt last);

    template <typename... Args>
    T* emplace(T* pos, Args&&... args);

    T* erase(T* pos);
    T* erase(T* first, T* last);

    T& at(size_type pos);
    T& operator[](size_type pos);
    T& front();
    T& back();
    void push_back(const T& value);

    template <typename... Args>
    void emplace_back(Args&&... args);

    void pop_back();

    template <typename InputIt>
    void append_range(InputIt first, InputIt last);

    size_type size() const noexcept;
    size_type capacity() const noexcept;
    size_type max_size() const noexcept;
    void reserve(size_type new_cap);
    void resize(size_type count, const T& value);
    void shrink_to_fit();
    void clear() noexcept;
    bool empty() const noexcept;
    T* data() noexcept;
    T* begin() noexcept;
    const T* cbegin() const noexcept;
    T* end() noexcept;
    const T* cend() const noexcept;
    std::reverse_iterator<T*> rbegin() noexcept;
    std::reverse_iterator<const T*> crbegin() const noexcept;
    std::reverse_iterator<T*> rend() noexcept;
    std::reverse_iterator<const T*> crend() const noexcept;

private:
    void reset() noexcept;

    std::unique_ptr<T[]> data_;
    size_type size_;
    size_type capacity_;
};

template <typename T>
bool operator==(const CustomVector<T>& lhs, const CustomVector<T>& rhs);

template <typename T>
bool operator!=(const CustomVector<T>& lhs, const CustomVector<T>& rhs);

template <typename T>
bool operator<(const CustomVector<T>& lhs, const CustomVector<T>& rhs);

template <typename T>
bool operator<=(const CustomVector<T>& lhs, const CustomVector<T>& rhs);

template <typename T>
bool operator>(const CustomVector<T>& lhs, const CustomVector<T>& rhs);

template <typename T>
bool operator>=(const CustomVector<T>& lhs, const CustomVector<T>& rhs);

#include "custom-vector.tpp"

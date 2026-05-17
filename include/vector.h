#pragma once

#include <cstddef>
#include <initializer_list>

template <typename T>
class Vector {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;

    Vector() noexcept;
    explicit Vector(size_type count, const T& value = T());
    Vector(std::initializer_list<T> init);
    Vector(const Vector& other);
    Vector& operator=(const Vector& other);
    ~Vector();

    [[nodiscard]] bool empty() const noexcept;
    size_type size() const noexcept;
    size_type capacity() const noexcept;

    reference operator[](size_type index) noexcept;
    const_reference operator[](size_type index) const noexcept;

    reference at(size_type index);
    const_reference at(size_type index) const;

    reference front();
    const_reference front() const;

    reference back();
    const_reference back() const;

    T* data() noexcept;
    const T* data() const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    void clear() noexcept;
    void reserve(size_type new_capacity);
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();
    void swap(Vector& other) noexcept;

private:
    T* data_;
    size_type size_;
    size_type capacity_;

    void destroy_at(size_type index) noexcept;
    void destroy_elements(size_type first, size_type last) noexcept;
    void clear_storage() noexcept;
};

template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept;

#include "vector.tpp"

#include <new>
#include <stdexcept>
#include <utility>

template <typename T>
Vector<T>::Vector() noexcept : data_(nullptr), size_(0), capacity_(0) {}

template <typename T>
Vector<T>::Vector(size_type count, const T& value)
    : data_(nullptr), size_(0), capacity_(0) {
    reserve(count);
    for (size_type i = 0; i < count; ++i) {
        push_back(value);
    }
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> init) : data_(nullptr), size_(0), capacity_(0) {
    reserve(init.size());
    for (const T& value : init) {
        push_back(value);
    }
}

template <typename T>
Vector<T>::Vector(const Vector& other) : data_(nullptr), size_(0), capacity_(0) {
    reserve(other.size_);
    for (size_type i = 0; i < other.size_; ++i) {
        push_back(other.data_[i]);
    }
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (this != &other) {
        Vector temp(other);
        swap(temp);
    }
    return *this;
}

template <typename T>
Vector<T>::~Vector() {
    clear_storage();
}

template <typename T>
bool Vector<T>::empty() const noexcept {
    return size_ == 0;
}

template <typename T>
typename Vector<T>::size_type Vector<T>::size() const noexcept {
    return size_;
}

template <typename T>
typename Vector<T>::size_type Vector<T>::capacity() const noexcept {
    return capacity_;
}

template <typename T>
typename Vector<T>::reference Vector<T>::operator[](size_type index) noexcept {
    return data_[index];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::operator[](size_type index) const noexcept {
    return data_[index];
}

template <typename T>
typename Vector<T>::reference Vector<T>::at(size_type index) {
    if (index >= size_) {
        throw std::out_of_range("Vector::at");
    }
    return data_[index];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::at(size_type index) const {
    if (index >= size_) {
        throw std::out_of_range("Vector::at");
    }
    return data_[index];
}

template <typename T>
typename Vector<T>::reference Vector<T>::front() {
    return data_[0];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::front() const {
    return data_[0];
}

template <typename T>
typename Vector<T>::reference Vector<T>::back() {
    return data_[size_ - 1];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::back() const {
    return data_[size_ - 1];
}

template <typename T>
T* Vector<T>::data() noexcept {
    return data_;
}

template <typename T>
const T* Vector<T>::data() const noexcept {
    return data_;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::begin() noexcept {
    return data_;
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::begin() const noexcept {
    return data_;
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::cbegin() const noexcept {
    return data_;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::end() noexcept {
    return data_ + size_;
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::end() const noexcept {
    return data_ + size_;
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::cend() const noexcept {
    return data_ + size_;
}

template <typename T>
void Vector<T>::clear() noexcept {
    destroy_elements(0, size_);
    size_ = 0;
}

template <typename T>
void Vector<T>::reserve(size_type new_capacity) {
    if (new_capacity <= capacity_) {
        return;
    }
    T* new_data = static_cast<T*>(::operator new(sizeof(T) * new_capacity));
    for (size_type i = 0; i < size_; ++i) {
        new (new_data + i) T(std::move_if_noexcept(data_[i]));
    }
    destroy_elements(0, size_);
    ::operator delete(data_);
    data_ = new_data;
    capacity_ = new_capacity;
}

template <typename T>
void Vector<T>::push_back(const T& value) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (data_ + size_) T(value);
    ++size_;
}

template <typename T>
void Vector<T>::push_back(T&& value) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (data_ + size_) T(std::move(value));
    ++size_;
}

template <typename T>
void Vector<T>::pop_back() {
    if (size_ == 0) {
        return;
    }
    destroy_at(size_ - 1);
    --size_;
}

template <typename T>
void Vector<T>::swap(Vector& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

template <typename T>
void Vector<T>::destroy_at(size_type index) noexcept {
    data_[index].~T();
}

template <typename T>
void Vector<T>::destroy_elements(size_type first, size_type last) noexcept {
    while (last > first) {
        destroy_at(--last);
    }
}

template <typename T>
void Vector<T>::clear_storage() noexcept {
    destroy_elements(0, size_);
    ::operator delete(data_);
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}

template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept {
    lhs.swap(rhs);
}

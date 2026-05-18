#include <algorithm>
#include <limits>
#include <new>
#include <stdexcept>
#include <utility>

template <typename T>
CustomVector<T>::CustomVector() noexcept : data_(), size_(0), capacity_(0) {}

template <typename T>
CustomVector<T>::CustomVector(const CustomVector& other)
    : data_(other.size_ == 0 ? nullptr : std::make_unique<T[]>(other.capacity_)),
      size_(other.size_),
      capacity_(other.size_ == 0 ? 0 : other.capacity_) {
    for (size_type i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

template <typename T>
CustomVector<T>::CustomVector(CustomVector&& other) noexcept
    : data_(std::move(other.data_)), size_(other.size_), capacity_(other.capacity_) {
    other.size_ = 0;
    other.capacity_ = 0;
}

template <typename T>
CustomVector<T>& CustomVector<T>::operator=(const CustomVector& other) {
    if (this != &other) {
        CustomVector temp(other);
        swap(temp);
    }
    return *this;
}

template <typename T>
CustomVector<T>& CustomVector<T>::operator=(CustomVector&& other) noexcept {
    if (this != &other) {
        reset();
        data_ = std::move(other.data_);
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

template <typename T>
void CustomVector<T>::swap(CustomVector& other) noexcept {
    using std::swap;
    data_.swap(other.data_);
    swap(size_, other.size_);
    swap(capacity_, other.capacity_);
}

template <typename T>
void CustomVector<T>::assign(size_type count, const T& value) {
    reset();

    if (count == 0) {
        return;
    }

    data_ = std::make_unique<T[]>(count);
    size_ = count;
    capacity_ = count;

    for (size_type i = 0; i < count; ++i) {
        data_[i] = value;
    }
}

template <typename T>
T* CustomVector<T>::insert(T* pos, const T& value) {
    if (pos < begin() || pos > end()) {
        throw std::out_of_range("CustomVector::insert");
    }
    if (size_ >= max_size()) {
        throw std::length_error("CustomVector::insert");
    }

    const size_type index = static_cast<size_type>(pos - begin());

    if (size_ >= capacity_) {
        const size_type new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
        reserve(new_cap);
    }

    for (size_type i = size_; i > index; --i) {
        data_[i] = std::move(data_[i - 1]);
    }
    data_[index] = value;
    ++size_;
    return data_.get() + index;
}

template <typename T>
template <typename InputIt>
T* CustomVector<T>::insert_range(T* pos, InputIt first, InputIt last) {
    if (pos < begin() || pos > end()) {
        throw std::out_of_range("CustomVector::insert_range");
    }

    const size_type count = static_cast<size_type>(std::distance(first, last));
    if (count == 0) {
        return pos;
    }
    if (size_ > max_size() - count) {
        throw std::length_error("CustomVector::insert_range");
    }

    const size_type index = static_cast<size_type>(pos - begin());
    const size_type new_size = size_ + count;

    if (new_size > capacity_) {
        const size_type new_cap =
            capacity_ == 0 ? new_size : (std::max)(new_size, capacity_ * 2);
        reserve(new_cap);
    }

    for (size_type i = size_; i > index; --i) {
        data_[i + count - 1] = std::move(data_[i - 1]);
    }

    size_type i = index;
    for (InputIt it = first; it != last; ++it, ++i) {
        data_[i] = *it;
    }

    size_ = new_size;
    return data_.get() + index;
}

template <typename T>
template <typename... Args>
T* CustomVector<T>::emplace(T* pos, Args&&... args) {
    if (pos < begin() || pos > end()) {
        throw std::out_of_range("CustomVector::emplace");
    }
    if (size_ >= max_size()) {
        throw std::length_error("CustomVector::emplace");
    }

    const size_type index = static_cast<size_type>(pos - begin());

    if (size_ >= capacity_) {
        const size_type new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
        reserve(new_cap);
    }

    for (size_type i = size_; i > index; --i) {
        data_[i] = std::move(data_[i - 1]);
    }

    data_[index].~T();
    new (data_.get() + index) T(std::forward<Args>(args)...);
    ++size_;
    return data_.get() + index;
}

template <typename T>
T* CustomVector<T>::erase(T* pos) {
    if (pos < begin() || pos >= end()) {
        throw std::out_of_range("CustomVector::erase");
    }
    return erase(pos, pos + 1);
}

template <typename T>
T* CustomVector<T>::erase(T* first, T* last) {
    if (first < begin() || first > end() || last < begin() || last > end() || first > last) {
        throw std::out_of_range("CustomVector::erase");
    }

    const size_type first_index = static_cast<size_type>(first - begin());
    const size_type last_index = static_cast<size_type>(last - begin());
    const size_type count = last_index - first_index;
    if (count == 0) {
        return first;
    }

    const size_type old_size = size_;
    const size_type new_size = old_size - count;

    for (size_type i = 0; i < old_size - last_index; ++i) {
        data_[first_index + i] = std::move(data_[last_index + i]);
    }

    for (size_type i = new_size; i < old_size; ++i) {
        data_[i].~T();
    }

    size_ = new_size;
    return data_.get() + first_index;
}

template <typename T>
T& CustomVector<T>::at(size_type pos) {
    if (pos >= size_) {
        throw std::out_of_range("CustomVector::at");
    }
    return data_[pos];
}

template <typename T>
T& CustomVector<T>::operator[](size_type pos) {
    return data_[pos];
}

template <typename T>
T& CustomVector<T>::front() {
    return data_[0];
}

template <typename T>
T& CustomVector<T>::back() {
    return data_[size_ - 1];
}

template <typename T>
void CustomVector<T>::push_back(const T& value) {
    if (size_ >= capacity_) {
        const size_type new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
        reserve(new_cap);
    }
    data_[size_++] = value;
}

template <typename T>
template <typename... Args>
void CustomVector<T>::emplace_back(Args&&... args) {
    if (size_ >= capacity_) {
        const size_type new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
        reserve(new_cap);
    }
    data_[size_++] = T(std::forward<Args>(args)...);
}

template <typename T>
void CustomVector<T>::pop_back() {
    data_[--size_].~T();
}

template <typename T>
template <typename InputIt>
void CustomVector<T>::append_range(InputIt first, InputIt last) {
    insert_range(end(), first, last);
}

template <typename T>
typename CustomVector<T>::size_type CustomVector<T>::size() const noexcept {
    return size_;
}

template <typename T>
typename CustomVector<T>::size_type CustomVector<T>::capacity() const noexcept {
    return capacity_;
}

template <typename T>
typename CustomVector<T>::size_type CustomVector<T>::max_size() const noexcept {
    return (std::min)(std::numeric_limits<size_type>::max(),
                      static_cast<size_type>(std::numeric_limits<std::ptrdiff_t>::max())) /
           sizeof(T);
}

template <typename T>
void CustomVector<T>::reserve(size_type new_cap) {
    if (new_cap <= capacity_) {
        return;
    }
    if (new_cap > max_size()) {
        throw std::length_error("CustomVector::reserve");
    }

    auto new_data = std::make_unique<T[]>(new_cap);
    for (size_type i = 0; i < size_; ++i) {
        new_data[i] = std::move(data_[i]);
    }
    data_ = std::move(new_data);
    capacity_ = new_cap;
}

template <typename T>
void CustomVector<T>::resize(size_type count, const T& value) {
    if (count > max_size()) {
        throw std::length_error("CustomVector::resize");
    }

    if (count < size_) {
        for (size_type i = count; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = count;
        return;
    }

    if (count > capacity_) {
        const size_type new_cap =
            capacity_ == 0 ? count : (std::max)(count, capacity_ * 2);
        reserve(new_cap);
    }

    for (size_type i = size_; i < count; ++i) {
        data_[i] = value;
    }
    size_ = count;
}

template <typename T>
void CustomVector<T>::shrink_to_fit() {
    if (size_ == capacity_) {
        return;
    }
    if (size_ == 0) {
        reset();
        return;
    }

    auto new_data = std::make_unique<T[]>(size_);
    for (size_type i = 0; i < size_; ++i) {
        new_data[i] = std::move(data_[i]);
    }
    data_ = std::move(new_data);
    capacity_ = size_;
}

template <typename T>
void CustomVector<T>::clear() noexcept {
    for (size_type i = 0; i < size_; ++i) {
        data_[i].~T();
    }
    size_ = 0;
}

template <typename T>
bool CustomVector<T>::empty() const noexcept {
    return size_ == 0;
}

template <typename T>
T* CustomVector<T>::data() noexcept {
    return data_.get();
}

template <typename T>
T* CustomVector<T>::begin() noexcept {
    return data_.get();
}

template <typename T>
const T* CustomVector<T>::cbegin() const noexcept {
    return data_.get();
}

template <typename T>
T* CustomVector<T>::end() noexcept {
    return data_.get() + size_;
}

template <typename T>
const T* CustomVector<T>::cend() const noexcept {
    return data_.get() + size_;
}

template <typename T>
std::reverse_iterator<T*> CustomVector<T>::rbegin() noexcept {
    return std::reverse_iterator<T*>(end());
}

template <typename T>
std::reverse_iterator<const T*> CustomVector<T>::crbegin() const noexcept {
    return std::reverse_iterator<const T*>(cend());
}

template <typename T>
std::reverse_iterator<T*> CustomVector<T>::rend() noexcept {
    return std::reverse_iterator<T*>(begin());
}

template <typename T>
std::reverse_iterator<const T*> CustomVector<T>::crend() const noexcept {
    return std::reverse_iterator<const T*>(cbegin());
}

template <typename T>
void CustomVector<T>::reset() noexcept {
    data_.reset();
    size_ = 0;
    capacity_ = 0;
}

template <typename T>
bool operator==(const CustomVector<T>& lhs, const CustomVector<T>& rhs) {
    return lhs.size() == rhs.size() &&
           std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

template <typename T>
bool operator!=(const CustomVector<T>& lhs, const CustomVector<T>& rhs) {
    return !(lhs == rhs);
}

template <typename T>
bool operator<(const CustomVector<T>& lhs, const CustomVector<T>& rhs) {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <typename T>
bool operator>(const CustomVector<T>& lhs, const CustomVector<T>& rhs) {
    return rhs < lhs;
}

template <typename T>
bool operator<=(const CustomVector<T>& lhs, const CustomVector<T>& rhs) {
    return !(rhs < lhs);
}

template <typename T>
bool operator>=(const CustomVector<T>& lhs, const CustomVector<T>& rhs) {
    return !(lhs < rhs);
}

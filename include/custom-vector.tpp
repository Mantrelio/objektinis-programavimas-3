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
void CustomVector<T>::reset() noexcept {
    data_.reset();
    size_ = 0;
    capacity_ = 0;
}

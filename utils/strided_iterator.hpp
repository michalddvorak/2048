#include <iterator>

template <typename T>
class strided_iterator
{
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using pointer = T*;
    
    strided_iterator() = default;
    
    strided_iterator(pointer ptr, difference_type stride)
            : ptr_(ptr), stride_(stride) { }
    
    reference operator*() noexcept { return *ptr_; }
    
    reference operator*() const noexcept { return *ptr_; }
    
    pointer operator->() const noexcept { return ptr_; }
    
    strided_iterator& operator++() noexcept
    {
        ptr_ += stride_;
        return *this;
    }
    
    strided_iterator& operator--() noexcept
    {
        ptr_ -= stride_;
        return *this;
    }
    
    strided_iterator operator++(int) noexcept
    {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }
    
    strided_iterator operator--(int) noexcept
    {
        auto tmp = *this;
        --(*this);
        return tmp;
    }
    
    // Arithmetic
    strided_iterator& operator+=(difference_type n) noexcept
    {
        ptr_ += n * stride_;
        return *this;
    }
    
    strided_iterator& operator-=(difference_type n) noexcept
    {
        ptr_ -= n * stride_;
        return *this;
    }
    
    reference operator[](difference_type i)
    {
        return *(*this + i);
    }
    
    reference operator[](difference_type i) const
    {
        return *(*this + i);
    }
    
    friend strided_iterator operator+(strided_iterator it, difference_type n) noexcept
    {
        it += n;
        return it;
    }
    
    friend strided_iterator operator+(difference_type n, strided_iterator it) noexcept
    {
        return it + n;
    }
    
    friend strided_iterator operator-(strided_iterator it, difference_type n) noexcept
    {
        it -= n;
        return it;
    }
    
    friend difference_type operator-(const strided_iterator& a,
                                     const strided_iterator& b) noexcept
    {
        return (a.ptr_ - b.ptr_) / a.stride_;
    }
    
    friend auto operator<=>(const strided_iterator& a,
                            const strided_iterator& b) noexcept
    {
        return a.ptr_ <=> b.ptr_;
    }
    
    friend bool operator==(const strided_iterator& a,
                           const strided_iterator& b) noexcept
    {
        return (a <=> b) == 0;
    }
  
  private:
    pointer ptr_ = nullptr;
    difference_type stride_ = 1;
};

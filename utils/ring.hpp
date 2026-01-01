#pragma once
#include <vector>

template <typename T>
class ring : std::vector<T>
{
  public:
    ring(std::vector<T> list) : list_(std::move(list)) { }
    
    void next()
    {
        if (++iter_ == list_.size())
            iter_ = 0;
    }
    
    void prev()
    {
        if (--iter_ == size_t(-1))
            iter_ = list_.size() - 1;
    }
    
    void operator--()
    {
        prev();
    }
    
    void operator++()
    {
        next();
    }
    
    T& operator*()
    {
        return get();
    }
    
    const T& operator*() const
    {
        return get();
    }
    
    T& get()
    {
        return list_[iter_];
    }
    
    const T& get() const
    {
        return list_[iter_];
    }
    
    auto begin()
    {
        return list_.begin();
    }
    auto end()
    {
        return list_.begin();
    }
    auto begin() const
    {
        return list_.begin();
    }
    auto end() const
    {
        return list_.end();
    }
  
    [[nodiscard]] size_t get_pos() const
    {
        return iter_;
    }
  private:
    std::vector<T> list_;
    size_t iter_ = 0;
};
#pragma once

#include <concepts>
#include <utility>

template <std::integral T>
class modulo_counter_circular
{
  public:
    modulo_counter_circular(T mod, T init = T()) : mod_(mod),
                                                   value_(init) { }
    
    operator T() { return value_; }
    
    void operator++()
    {
        if (++value_ == mod_)
            value_ = T(0);
    }
    
    void operator--()
    {
        if (--value_ == T(-1))
            value_ = mod_ - 1;
    }
  
  private:
    
    T mod_;
    T value_;
};

template <std::integral T>
class modulo_counter_saturated
{
  public:
    modulo_counter_saturated(T mod, T init = T()) : mod_(mod),
                                                    value_(init) { }
    
    void operator++()
    {
        if (value_ != this->mod_ - 1)
            ++value_;
    }
    
    void operator--()
    {
        if (value_ != T(0))
            --value_;
    }
  
  private:
    T mod_;
    T value_;
};

#pragma once

#include <vector>
#include <ranges>

template <typename T>
class matrix : public std::vector<T>
{
  public:
    using value_type = T;
    using base_type = std::vector<T>;
    
    matrix(size_t rows, size_t cols, const T& value = T()) : std::vector<T>(rows * cols, value),
                                                             m_rows(rows),
                                                             m_cols(cols) { }
    
    size_t num_rows() const
    {
        return m_rows;
    }
    
    size_t num_columns() const
    {
        return m_cols;
    }
    
    T* operator[](size_t i)
    {
        return std::vector<T>::data() + i * m_cols;
    }
    
    const T* operator[](size_t i) const
    {
        return std::vector<T>::data() + i * m_cols;
    }
  
  private:
    size_t m_rows;
    size_t m_cols;
};
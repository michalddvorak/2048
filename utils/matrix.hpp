#pragma once

#include <vector>
#include <ranges>

template <typename T>
class matrix
{
  public:
    using value_type = T;
    
    matrix(size_t rows, size_t cols) : m_data(rows * cols),
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
        return m_data.data() + i * m_cols;
    }
    
    const T* operator[](size_t i) const
    {
        return m_data.data() + i * m_cols;
    }
    
    auto begin() const
    {
        return m_data.begin();
    }
    
    auto end() const
    {
        return m_data.end();
    }
    
    auto begin()
    {
        return m_data.begin();
    }
    
    auto end()
    {
        return m_data.end();
    }
    
    T* data()
    {
        return m_data.data();
    }
    
    const T* data() const
    {
        return m_data.data();
    }
  
  private:
    std::vector<T> m_data;
    size_t m_rows;
    size_t m_cols;
};
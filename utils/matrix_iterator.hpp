#pragma once

#include <memory>
#include "matrix.hpp"


template <typename Self>
class incrementable
{
  public:
    template <typename ...Ts>
    decltype(auto) operator++(Ts...)
    {
        std::conditional_t<sizeof...(Ts), Self, Self&> that = static_cast<Self&>(*this);
        static_cast<Self*>(this)->do_increment();
        return that;
    }
};


template <typename Matrix>
class matrix_iterator
{
  public:
    matrix_iterator(Matrix* matrix, size_t i, size_t j) : m_matrix(matrix),
                                                          m_i(i),
                                                          m_j(j) { }
    
    typename Matrix::value_type& operator*() const
    {
        return (*m_matrix)[m_i][m_j];
    }
    
    friend auto operator<=>(const matrix_iterator&, const matrix_iterator&) = default;
  
  protected:
    Matrix* m_matrix;
    size_t m_i;
    size_t m_j;
};

template <typename Matrix>
class const_matrix_iterator
{
  public:
    const_matrix_iterator(const Matrix* matrix, size_t i, size_t j) : m_matrix(matrix),
                                                                      m_i(i),
                                                                      m_j(j) { }
    
    const typename Matrix::value_type& operator*() const
    {
        return (*m_matrix)[m_i][m_j];
    }
    
    friend auto operator<=>(const const_matrix_iterator&, const const_matrix_iterator&) = default;
  
  protected:
    const Matrix* m_matrix;
    size_t m_i;
    size_t m_j;
};


#define DECLARE_MATRIX_ITERATOR(name, increment_logic) \
template<typename Matrix> \
class name : public matrix_iterator<Matrix>, public incrementable<name<Matrix>> \
{ \
   public:                          \
    using matrix_iterator<Matrix>::matrix_iterator;                            \
    void do_increment()          \
        increment_logic                       \
};                                  \
template<typename Matrix> \
class const_##name : public const_matrix_iterator<Matrix>, public incrementable<const_##name<Matrix>> \
{ \
   public:                          \
    using const_matrix_iterator<Matrix>::const_matrix_iterator;                            \
    void do_increment()        \
        increment_logic                       \
};                                  \



DECLARE_MATRIX_ITERATOR(forward_column_iterator, { ++this->m_i; })



DECLARE_MATRIX_ITERATOR(forward_row_iterator, { ++this->m_j; })

DECLARE_MATRIX_ITERATOR(reverse_column_iterator, { --this->m_i; })

DECLARE_MATRIX_ITERATOR(reverse_row_iterator, { --this->m_j; })

DECLARE_MATRIX_ITERATOR(row_lr_iterator,
                        {
                     if (this->m_j == this->m_matrix->num_cols() - 1) {
                         ++this->m_i;
                         this->m_j = 0;
                     }
                     else
                         ++this->m_j;
                 }
)


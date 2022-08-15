#pragma once

#include <memory>
#include "matrix.hpp"

template<typename Matrix>
class matrix_iterator
{
 public:
	matrix_iterator(Matrix& matrix, size_t i, size_t j) : m_matrix(matrix),
														  m_i(i),
														  m_j(j) { }
	typename Matrix::value_type& operator *()
	{
		return m_matrix[m_i][m_j];
	}
	friend bool operator ==(const matrix_iterator& x, const matrix_iterator& y)
	{
		return &x.m_matrix == &y.m_matrix && x.m_i == y.m_i && x.m_j == y.m_j;
	}
	friend bool operator !=(const matrix_iterator& x, const matrix_iterator& y)
	{
		return !(x == y);
	}
 protected:
	Matrix& m_matrix;
	size_t m_i;
	size_t m_j;
};

template<typename Matrix>
class const_matrix_iterator
{
 public:
	const_matrix_iterator(const Matrix& matrix, size_t i, size_t j) : m_matrix(matrix),
																	  m_i(i),
																	  m_j(j) { }
	const typename Matrix::value_type& operator *()
	{
		return m_matrix[m_i][m_j];
	}
	friend bool operator ==(const const_matrix_iterator& x, const const_matrix_iterator& y)
	{
		return &x.m_matrix == &y.m_matrix && x.m_i == y.m_i && x.m_j == y.m_j;
	}
	friend bool operator !=(const const_matrix_iterator& x, const const_matrix_iterator& y)
	{
		return !(x == y);
	}
 protected:
	const Matrix& m_matrix;
	size_t m_i;
	size_t m_j;
};

template<typename Matrix>
class forward_column_iterator : public matrix_iterator<Matrix>
{
 public:
	forward_column_iterator(Matrix& matrix, size_t i, size_t j) : matrix_iterator<Matrix>(matrix, i, j) { }
	
	template<typename ...Ts>
	decltype(auto) operator ++(Ts...)
	{
		std::conditional_t<sizeof...(Ts), forward_column_iterator, forward_column_iterator&> that = *this;
		++this->m_i;
		return that;
	}
};


template<typename Matrix>
class const_forward_column_iterator : public const_matrix_iterator<Matrix>
{
 public:
	const_forward_column_iterator(const Matrix& matrix, size_t i, size_t j) : const_matrix_iterator<Matrix>(matrix, i, j) { }
	
	template<typename ...Ts>
	decltype(auto) operator ++(Ts...)
	{
		std::conditional_t<sizeof...(Ts), const_forward_column_iterator, const_forward_column_iterator&> that = *this;
		++this->m_i;
		return that;
	}
};

template<typename Matrix>
class forward_row_iterator : public matrix_iterator<Matrix>
{
 public:
	forward_row_iterator(Matrix& matrix, size_t i, size_t j) : matrix_iterator<Matrix>(matrix, i, j) { }
	
	template<typename ...Ts>
	decltype(auto) operator ++(Ts...)
	{
		std::conditional_t<sizeof...(Ts), forward_row_iterator, forward_row_iterator&> that = *this;
		++this->m_j;
		return that;
	}
};


template<typename Matrix>
class const_forward_row_iterator : public const_matrix_iterator<Matrix>
{
 public:
	const_forward_row_iterator(const Matrix& matrix, size_t i, size_t j) : const_matrix_iterator<Matrix>(matrix, i, j) { }
	
	template<typename ...Ts>
	decltype(auto) operator ++(Ts...)
	{
		std::conditional_t<sizeof...(Ts), const_forward_row_iterator, const_forward_row_iterator&> that = *this;
		++this->m_j;
		return that;
	}
};


template<typename Matrix>
class reverse_column_iterator : public matrix_iterator<Matrix>
{
 public:
	reverse_column_iterator(Matrix& matrix, size_t i, size_t j) : matrix_iterator<Matrix>(matrix, i, j) { }
	
	template<typename ...Ts>
	decltype(auto) operator ++(Ts...)
	{
		std::conditional_t<sizeof...(Ts), reverse_column_iterator, reverse_column_iterator&> that = *this;
		--this->m_i;
		return that;
	}
};


template<typename Matrix>
class const_reverse_column_iterator : public const_matrix_iterator<Matrix>
{
 public:
	const_reverse_column_iterator(const Matrix& matrix, size_t i, size_t j) : const_matrix_iterator<Matrix>(matrix, i, j) { }
	
	template<typename ...Ts>
	decltype(auto) operator ++(Ts...)
	{
		std::conditional_t<sizeof...(Ts), const_reverse_column_iterator, const_reverse_column_iterator&> that = *this;
		--this->m_i;
		return that;
	}
};

template<typename Matrix>
class reverse_row_iterator : public matrix_iterator<Matrix>
{
 public:
	reverse_row_iterator(Matrix& matrix, size_t i, size_t j) : matrix_iterator<Matrix>(matrix, i, j) { }
	
	template<typename ...Ts>
	decltype(auto) operator ++(Ts...)
	{
		std::conditional_t<sizeof...(Ts), reverse_row_iterator, reverse_row_iterator&> that = *this;
		--this->m_j;
		return that;
	}
};


template<typename Matrix>
class const_reverse_row_iterator : public const_matrix_iterator<Matrix>
{
 public:
	const_reverse_row_iterator(const Matrix& matrix, size_t i, size_t j) : const_matrix_iterator<Matrix>(matrix, i, j) { }
	
	template<typename ...Ts>
	decltype(auto) operator ++(Ts...)
	{
		std::conditional_t<sizeof...(Ts), const_reverse_row_iterator, const_reverse_row_iterator&> that = *this;
		--this->m_j;
		return that;
	}
};


template<typename Matrix>
class row_lr_iterator : public matrix_iterator<Matrix>
{
 public:
	row_lr_iterator(Matrix& matrix, size_t i, size_t j) : matrix_iterator<Matrix>(matrix, i, j) { }
	
	template<typename ...Ts>
	decltype(auto) operator ++(Ts...)
	{
		std::conditional_t<sizeof...(Ts), row_lr_iterator, row_lr_iterator&> that = *this;
		if(this->m_j == this->m_matrix.num_cols() - 1)
		{
			++this->m_i;
			this->m_j = 0;
		}
		else
			++this->m_j;
		return that;
	}
};


template<typename Matrix>
class const_row_lr_iterator : public const_matrix_iterator<Matrix>
{
 public:
	const_row_lr_iterator(const Matrix& matrix, size_t i, size_t j) : const_matrix_iterator<Matrix>(matrix, i, j) { }
	
	template<typename ...Ts>
	decltype(auto) operator ++(Ts...)
	{
		std::conditional_t<sizeof...(Ts), const_row_lr_iterator, const_row_lr_iterator&> that = *this;
		if(this->m_j == this->m_matrix.num_cols() - 1)
		{
			++this->m_i;
			this->m_j = 0;
		}
		else
			++this->m_j;
		return that;
	}
};
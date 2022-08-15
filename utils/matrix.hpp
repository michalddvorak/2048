#pragma once

#include <vector>
#include "matrix_iterator.hpp"

template<typename T>
class matrix
{
 public:
	using value_type = T;
	
	matrix(size_t rows, size_t cols) : m_arr(rows, std::vector<T>(cols)),
									   m_rows(rows),
									   m_cols(cols) { }
	size_t num_rows() const
	{
		return m_rows;
	}
	size_t num_cols() const
	{
		return m_cols;
	}
	std::vector<T>& operator [](size_t i)
	{
		return m_arr[i];
	}
	const std::vector<T>& operator [](size_t i) const
	{
		return m_arr[i];
	}
	
	forward_row_iterator<matrix> row_forward_begin(size_t i)
	{
		return forward_row_iterator<matrix>(*this, i, 0);
	}
	forward_column_iterator<matrix> column_forward_begin(size_t j)
	{
		return forward_column_iterator<matrix>(*this, 0, j);
	}
	reverse_row_iterator<matrix> row_reverse_begin(size_t i)
	{
		return reverse_row_iterator<matrix>(*this, i, m_cols - 1);
	}
	reverse_column_iterator<matrix> column_reverse_begin(size_t j)
	{
		return reverse_column_iterator<matrix>(*this, m_rows - 1, j);
	}
	row_lr_iterator<matrix> row_lr_begin()
	{
		return row_lr_iterator<matrix>(*this, 0, 0);
	}
	
	
	forward_row_iterator<matrix> row_forward_end(size_t i)
	{
		return forward_row_iterator<matrix>(*this, i, m_cols);
	}
	forward_column_iterator<matrix> column_forward_end(size_t j)
	{
		return forward_column_iterator<matrix>(*this, m_rows, j);
	}
	reverse_row_iterator<matrix> row_reverse_end(size_t i)
	{
		return reverse_row_iterator<matrix>(*this, i, -1);
	}
	reverse_column_iterator<matrix> column_reverse_end(size_t j)
	{
		return reverse_column_iterator<matrix>(*this, -1, j);
	}
	row_lr_iterator<matrix> row_lr_end()
	{
		return row_lr_iterator<matrix>(*this, m_rows, 0);
	}
	
	
	const_forward_row_iterator<matrix> row_forward_begin(size_t i) const
	{
		return const_forward_row_iterator<matrix>(*this, i, 0);
	}
	const_forward_column_iterator<matrix> column_forward_begin(size_t j) const
	{
		return const_forward_column_iterator<matrix>(*this, 0, j);
	}
	const_reverse_row_iterator<matrix> row_reverse_begin(size_t i) const
	{
		return const_reverse_row_iterator<matrix>(*this, i, m_cols - 1);
	}
	const_reverse_column_iterator<matrix> column_reverse_begin(size_t j) const
	{
		return const_reverse_column_iterator<matrix>(*this, m_rows - 1, j);
	}
	const_row_lr_iterator<matrix> row_lr_begin() const
	{
		return const_row_lr_iterator<matrix>(*this, 0, 0);
	}
	
	
	const_forward_row_iterator<matrix> row_forward_end(size_t i) const
	{
		return const_forward_row_iterator<matrix>(*this, i, m_cols);
	}
	const_forward_column_iterator<matrix> column_forward_end(size_t j) const
	{
		return const_forward_column_iterator<matrix>(*this, m_rows, j);
	}
	const_reverse_row_iterator<matrix> row_reverse_end(size_t i) const
	{
		return const_reverse_row_iterator<matrix>(*this, i, -1);
	}
	const_reverse_column_iterator<matrix> column_reverse_end(size_t j) const
	{
		return const_reverse_column_iterator<matrix>(*this, -1, j);
	}
	const_row_lr_iterator<matrix> row_lr_end() const
	{
		return const_row_lr_iterator<matrix>(*this, m_rows, 0);
	}
	
	auto begin()
	{
		return row_lr_begin();
	}
	auto end()
	{
		return row_lr_end();
	}
	auto begin() const
	{
		return row_lr_begin();
	}
	auto end() const
	{
		return row_lr_end();
	}
 private:
	std::vector<std::vector<T>> m_arr;
	size_t m_rows;
	size_t m_cols;
};





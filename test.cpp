#include <iostream>
#include <cassert>
#include "utils/matrix.hpp"
#include "utils/matrix_iterator.hpp"
#include "utils/utils.hpp"

using namespace std;

void basic_matrix_iter_test()
{
	/*
	 * 0 1 2
	 * 3 4 5
	 * 6 7 8
	 * */
	matrix<int> a(3, 3);
	int x = 0;
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			a[i][j] = x++;
	
	for(int i = 0; i < 3; ++i)
	{
		std::vector<int> expected = {3 * i, 3 * i + 1, 3 * i + 2};
		std::vector<int> actual;
		for(auto it = a.row_forward_begin(i); it != a.row_forward_end(i); ++it)
			actual.push_back(*it);
		assert(expected == actual);
	}
	
	for(int i = 0; i < 3; ++i)
	{
		std::vector<int> expected = {3 * i + 2, 3 * i + 1, 3 * i};
		std::vector<int> actual;
		for(auto it = a.row_reverse_begin(i); it != a.row_reverse_end(i); ++it)
			actual.push_back(*it);
		assert(expected == actual);
	}
	
	for(int j = 0; j < 3; ++j)
	{
		std::vector<int> expected = {j, 3 + j, 6 + j};
		std::vector<int> actual;
		for(auto it = a.column_forward_begin(j); it != a.column_forward_end(j); ++it)
			actual.push_back(*it);
		assert(expected == actual);
	}
	
	for(int j = 0; j < 3; ++j)
	{
		std::vector<int> expected = {6 + j, 3 + j, j};
		std::vector<int> actual;
		for(auto it = a.column_reverse_begin(j); it != a.column_reverse_end(j); ++it)
			actual.push_back(*it);
		assert(expected == actual);
	}
	
	
	std::vector<int> expected = {0, 1, 2, 3, 4, 5, 6, 7, 8};
	std::vector<int> actual;
	for(auto it = a.row_lr_begin(); it != a.row_lr_end(); ++it)
		actual.push_back(*it);
	
	assert(expected == actual);
	
	actual.clear();
	for(auto& v: a)
		actual.push_back(v);
	
	assert(expected == actual);
}

void more_matrix_iter_test()
{
	matrix<int> a(3, 10);
	int x = 0;
	
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 10; ++j)
			a[i][j] = x++;
	
	for(int i = 0; i < 3; ++i)
	{
		std::vector<int> expected;
		for(int j = 0; j < 10; ++j)
			expected.push_back(10 * i + j);
		
		std::vector<int> actual;
		for(auto it = a.row_forward_begin(i); it != a.row_forward_end(i); ++it)
			actual.push_back(*it);
		
		assert(expected == actual);
	}
	
	for(int i = 0; i < 3; ++i)
	{
		std::vector<int> expected;
		for(int j = 0; j < 10; ++j)
			expected.push_back(10 * i + 9 - j);
		
		std::vector<int> actual;
		for(auto it = a.row_reverse_begin(i); it != a.row_reverse_end(i); ++it)
			actual.push_back(*it);
		
		assert(expected == actual);
	}
	
	for(int j = 0; j < 10; ++j)
	{
		std::vector<int> expected = {j, 10 + j, 20 + j};
		
		std::vector<int> actual;
		for(auto it = a.column_forward_begin(j); it != a.column_forward_end(j); ++it)
			actual.push_back(*it);
		
		assert(expected == actual);
	}
	
	for(int j = 0; j < 3; ++j)
	{
		std::vector<int> expected = {20 + j, 10 + j, j};
		
		std::vector<int> actual;
		for(auto it = a.column_reverse_begin(j); it != a.column_reverse_end(j); ++it)
			actual.push_back(*it);
		
		assert(expected == actual);
	}
}

void merge_test()
{
	std::vector<int> b {2, 2, 2, 2};
	assert(merge(b.begin(), b.end()));
	{
		std::vector<int> expected = {4, 4, 0, 0};
		assert(b == expected);
	}
	assert(merge(b.begin(), b.end()));
	{
		std::vector<int> expected = {8, 0, 0, 0};
		assert(b == expected);
	}
	assert(!merge(b.begin(),b.end()));
	assert(!merge(b.begin(),b.end()));
	
	std::vector<int> c {1,2,3,4};
	assert(!merge(c.begin(),c.end()));
}


int main()
{
	basic_matrix_iter_test();
	more_matrix_iter_test();
	merge_test();
}
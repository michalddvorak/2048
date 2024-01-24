#pragma once

#include <cstddef>
#include <random>
#include "../utils/matrix.hpp"
#include "../io/io.hpp"

class game
{
 public:
	game(size_t rows, size_t cols, size_t range, size_t numgen, io& io);
	void run();
 
 private:
	void put_random();
	bool generate_random_empty_index(size_t& r, size_t& c);
	
	int score() const;
	bool move_left();
	bool move_right();
	bool move_down();
	bool move_up();
	bool is_lost() const;
	
	matrix<int> m_board;
	size_t m_range;
	size_t m_numgen;
	std::mt19937 m_gen = std::mt19937(std::random_device {}());
	io& m_io;
};

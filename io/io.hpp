#pragma once

#include "../utils/matrix.hpp"

class io
{
 public:
	virtual ~io() = default;
	virtual void clear_screen() = 0;
	virtual void print_board(const matrix<int>& board) = 0;
	virtual void handle_loss(const matrix<int>& board, int score) = 0;
	virtual void exit(const matrix<int>& board, int score) = 0;
	virtual int getc() = 0;
};


#pragma once

#include <termios.h>
#include "io.hpp"

class term_io : public io
{
 public:
	term_io(size_t height = 3, size_t width = 4);
	~term_io() override;
	void clear_screen() override;
	void exit(const matrix<int>& board, int score) override;
	void handle_loss(const matrix<int>& board, int score) override;
	void print_board(const matrix<int>& board) override;
	int getc() override;
 private:
	struct rgb
	{
		uint8_t r, g, b;
	};
	rgb get_color(size_t idx);
	void init_terminal();
	void restore_terminal();
	void move_cursor(size_t i, size_t j);
	void print_corner(size_t cols);
	void print_middle(const matrix<int>& board, size_t i, bool is_num);
	struct termios m_old_ttystate;
	size_t m_height;
	size_t m_width;
};





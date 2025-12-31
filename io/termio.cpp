#include <unistd.h>
#include <iostream>
#include <iomanip>
#include "termio.hpp"
#include "../utils/matrix_views.hpp"

term_io::term_io(size_t height, size_t width) : m_height(height),
												m_width(width)
{
	init_terminal();
}
term_io::~term_io()
{
	restore_terminal();
}
int term_io::getc()
{
	return std::cin.get();
}
void term_io::init_terminal()
{
	tcgetattr(STDIN_FILENO, &m_old_ttystate); //store old ttystate
	struct termios ttystate;
	tcgetattr(STDIN_FILENO, &ttystate);
	ttystate.c_lflag &= ~ICANON; //disable canonical mode
	ttystate.c_cc[VMIN] = 1; //react on 1 character
	ttystate.c_lflag &= ~ECHO; //don't echo pressed characters
	tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

void term_io::restore_terminal()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &m_old_ttystate);
}
void term_io::clear_screen()
{
	std::cout << "\033[2J";
}
void term_io::move_cursor(size_t i, size_t j)
{
	std::cout << "\033[" << i << ';' << j << 'H';
}
void term_io::print_board(const matrix<int>& board)
{
	move_cursor(1, 1);
	print_corner(board.num_columns());
	for(size_t i = 0; i < board.num_rows(); ++i)
	{
		for(size_t h = 0; h < m_height; ++h) //height
			print_middle(board, i, h == (m_height - 1) / 2);
		print_corner(board.num_columns());
	}
	std::cout.flush();
}
void term_io::print_corner(size_t cols)
{
	std::cout << '+';
	for(size_t j = 0; j < cols; ++j)
		std::cout << std::string(m_width, '-') << "+";
	std::cout << '\n';
}
void term_io::print_middle(const matrix<int>& board, size_t i, bool is_num)
{
	std::cout << '|';
    for(auto& x : row_view(&board,i))
	{
		if(x == 0)
			std::cout << std::setw(m_width) << ' ';
		else
		{
			auto color = get_color(__builtin_popcount(x - 1) - 1);
			std::cout << "\033[48;2;" << +color.r << ';' << +color.g << ';' << +color.b << 'm';
			std::cout << std::setw(m_width);
			if(is_num)
				std::cout << x;
			else
				std::cout << ' ';
			std::cout << "\033[0m";
		}
		std::cout << '|';
	}
	std::cout << '\n';
}
void term_io::exit(const matrix<int>& board, int score)
{
	clear_screen();
	print_board(board);
	std::cout << "exited, your score: " << score << std::endl;
}
void term_io::handle_loss(const matrix<int>& board, int score)
{
	clear_screen();
	print_board(board);
	std::cout << "you lost, your score: " << score << std::endl;
}
term_io::rgb term_io::get_color(size_t idx)
{
	const static rgb LIGHT_BLUE = {66, 135, 245};
	const static rgb DARK_BLUE = {1, 49, 125};
	const static rgb BROWN = {125, 65, 1};
	const static rgb YELLOW = {247, 210, 22};
	const static rgb LIGHT_GREEN = {146, 247, 22};
	const static rgb TEAL = {22, 247, 202};
	const static rgb PURPLE = {191, 22, 247};
	const static rgb LIGHT_RED = {191, 86, 86};
	const static rgb PINK = {255, 0, 204};
	const static rgb DARK_GREEN = {9, 117, 4};
	const static rgb ORANGE = {255, 170, 0};
	const static rgb DARK_PURPLE = {70, 8, 94};
	const static rgb REDRANGE = {235, 87, 38};
	
	const static std::array colors =
			{
					LIGHT_BLUE,
					DARK_BLUE,
					BROWN,
					ORANGE,
					DARK_GREEN,
					PURPLE,
					DARK_PURPLE,
					LIGHT_RED,
					PINK,
					REDRANGE,
					ORANGE
			};
	if(idx < colors.size())
		return colors[idx];
	else
		return {0, 0, 0};
}

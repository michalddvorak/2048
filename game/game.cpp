#include "game.hpp"
#include "../utils/utils.hpp"

game::game(size_t rows, size_t cols, size_t range, size_t numgen, io& io) : m_board(rows, cols),
																			m_range(range),
																			m_numgen(numgen),
																			m_io(io) { }


bool game::move_left()
{
	bool has_changed = false;
	for(size_t i = 0; i < m_board.num_rows(); ++i)
		has_changed = merge(m_board.row_forward_begin(i), m_board.row_forward_end(i)) || has_changed;
	return has_changed;
}

bool game::move_right()
{
	bool has_changed = false;
	for(size_t i = 0; i < m_board.num_rows(); ++i)
		has_changed = merge(m_board.row_reverse_begin(i), m_board.row_reverse_end(i)) || has_changed;
	return has_changed;
}
bool game::move_up()
{
	bool has_changed = false;
	for(size_t j = 0; j < m_board.num_cols(); ++j)
		has_changed = merge(m_board.column_forward_begin(j), m_board.column_forward_end(j)) || has_changed;
	return has_changed;
}
bool game::move_down()
{
	bool has_changed = false;
	for(size_t j = 0; j < m_board.num_cols(); ++j)
		has_changed = merge(m_board.column_reverse_begin(j), m_board.column_reverse_end(j)) || has_changed;
	return has_changed;
}
bool game::is_lost() const
{
	for(auto& val: m_board)
		if(val == 0)
			return false;
	
	for(size_t i = 0; i < m_board.num_rows(); ++i)
		for(auto jtn = m_board.row_forward_begin(i), jt = jtn++; jtn != m_board.row_forward_end(i); ++jt, ++jtn)
			if(*jtn == *jt)
				return false;
	
	for(size_t j = 0; j < m_board.num_cols(); ++j)
		for(auto itn = m_board.column_forward_begin(j), it = itn++; itn != m_board.column_forward_end(j); ++it, ++itn)
			if(*itn == *it)
				return false;
	
	return true;
}
int game::score() const
{
	int result = 0;
	for(auto& val: m_board)
		result += val;
	return result;
}

void game::run()
{
	m_io.clear_screen();
	put_random();
	m_io.print_board(m_board);
	while(true)
	{
		bool move = false;
		switch(m_io.getc())
		{
			case 'a':
				move = move_left();
				break;
			case 'd':
				move = move_right();
				break;
			case 's':
				move = move_down();
				break;
			case 'w':
				move = move_up();
				break;
			case 'x':
			case 'q':
				m_io.exit(m_board, score());
				return;
		}
		if(move)
		{
			put_random();
			m_io.print_board(m_board);
		}
		if(is_lost())
		{
			m_io.handle_loss(m_board,score());
			return;
		}
	}
}
bool game::generate_random_empty_index(size_t& r, size_t& c)
{
	size_t cnt = 0;
	for(size_t i = 0; i < m_board.num_rows(); ++i)
		for(size_t j = 0; j < m_board.num_cols(); ++j)
			if(m_board[i][j] == 0)
				cnt++;
	if(cnt == 0)
		return false;
	
	std::uniform_int_distribution<std::mt19937::result_type> distr(0, cnt - 1);
	auto offset = distr(m_gen);
	
	for(size_t i = 0; i < m_board.num_rows(); ++i)
		for(size_t j = 0; j < m_board.num_cols(); ++j)
			if(m_board[i][j] == 0 && offset-- == 0)
			{
				r = i;
				c = j;
				return true;
			}
	return true;
}
void game::put_random()
{
	std::uniform_int_distribution<std::mt19937::result_type> distr(1, m_range);
	for(size_t n = 0; n < m_numgen; ++n)
	{
		size_t i, j;
		if(!generate_random_empty_index(i, j))
			return;
		auto bit = distr(m_gen);
		m_board[i][j] = 1 << bit;
	}
}

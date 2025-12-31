#include "game.hpp"
#include <algorithm>
#include <ranges>
#include "../utils/utils.hpp"
#include "../utils/matrix_views.hpp"

game::game(size_t rows, size_t cols, size_t range, size_t numgen, io& io) : m_board(rows, cols),
                                                                            m_range(range),
                                                                            m_numgen(numgen),
                                                                            m_io(io) { }

template <typename InnerCompose = decltype(std::views::transform(std::identity {}))>
bool slide(std::ranges::range auto&& rng, InnerCompose compose_inner = std::views::transform(std::identity {}))
{
    auto&& transformed = std::views::transform(rng,
                                               [&](std::ranges::range auto&& r) { return merge(r | compose_inner); });
    return std::accumulate(transformed.begin(), transformed.end(), false, std::logical_or {});
}

bool game::move_left()
{
    return slide(rows(m_board));
}

bool game::move_right()
{
    return slide(rows(m_board), std::views::reverse);
}

bool game::move_up()
{
    return slide(columns(m_board));
}

bool game::move_down()
{
    return slide(columns(m_board), std::views::reverse);
}

bool game::is_lost() const
{
    auto is_zero = [](auto x) { return x == 0; };
    
    auto contains_two_adjacent_equal_elements = [](std::ranges::range auto&& rng)
    {
        auto it = std::ranges::adjacent_find(rng);
        return it != rng.end();
    };
    
    if (std::ranges::any_of(m_board, is_zero))
        return false;
    
    if (std::ranges::any_of(columns(m_board), contains_two_adjacent_equal_elements))
        return false;
    
    if (std::ranges::any_of(rows(m_board), contains_two_adjacent_equal_elements))
        return false;
    
    
    return true;
}

int game::score() const
{
    return std::accumulate(m_board.begin(), m_board.end(), 0);
}

void game::run()
{
    m_io.clear_screen();
    put_random();
    m_io.print_board(m_board);
    while (true) {
        bool move = false;
        switch (m_io.getc()) {
            case 'a': move = move_left();
                break;
            case 'd': move = move_right();
                break;
            case 's': move = move_down();
                break;
            case 'w': move = move_up();
                break;
            case 'x':
            case 'q': m_io.exit(m_board, score());
                return;
        }
        if (move) {
            put_random();
            m_io.print_board(m_board);
        }
        if (is_lost()) {
            m_io.handle_loss(m_board, score());
            return;
        }
    }
}

bool game::generate_random_empty_index(size_t& r, size_t& c)
{
    size_t cnt = 0;
    for (size_t i = 0; i < m_board.num_rows(); ++i)
        for (size_t j = 0; j < m_board.num_columns(); ++j)
            if (m_board[i][j] == 0)
                cnt++;
    if (cnt == 0)
        return false;
    
    std::uniform_int_distribution<std::mt19937::result_type> distr(0, cnt - 1);
    auto offset = distr(m_gen);
    
    for (size_t i = 0; i < m_board.num_rows(); ++i)
        for (size_t j = 0; j < m_board.num_columns(); ++j)
            if (m_board[i][j] == 0 && offset-- == 0) {
                r = i;
                c = j;
                return true;
            }
    return true;
}

void game::put_random()
{
    std::uniform_int_distribution<std::mt19937::result_type> distr(1, m_range);
    for (size_t n = 0; n < m_numgen; ++n) {
        size_t i, j;
        if (!generate_random_empty_index(i, j))
            return;
        auto bit = distr(m_gen);
        m_board[i][j] = 1 << bit;
    }
}

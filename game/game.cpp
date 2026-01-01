#include "game.hpp"
#include <algorithm>
#include <ranges>
#include <fstream>

#include "../utils/utils.hpp"
#include "../utils/matrix_views.hpp"

game::game(size_t rows, size_t cols, size_t range, size_t numgen, io& io) :
        main_menu_(io,
                   {
                           {"Play",        [&]() { play_game(); }},
                           {"High-Scores", [&]() { show_highscores(); }},
                           {"Options",     [&]() { options(); }},
                           {"Exit",        [&]() { exit_ = true; }}
                   }),
        m_board(rows, cols),
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
    auto contains_two_adjacent_equal_elements = [](std::ranges::range auto&& rng)
    {
        auto it = std::ranges::adjacent_find(rng);
        return it != rng.end();
    };
    
    if (std::ranges::any_of(m_board, is_zero) ||
        std::ranges::any_of(columns(m_board), contains_two_adjacent_equal_elements) ||
        std::ranges::any_of(rows(m_board), contains_two_adjacent_equal_elements))
        return false;
    
    return true;
}

int game::score() const
{
    return std::accumulate(m_board.begin(), m_board.end(), 0);
}


game::score_t game::run()
{
    for (auto& x: m_board)x = 0; //TODO
    m_io.clear_screen();
    put_random();
    m_io.print_board(m_board);
    while (true) {
        bool move = false;
        auto key = m_io.get_key();
        switch (key.type) {
            case EKEY::LEFTARROW: move = move_left();
                break;
            case EKEY::RIGHTARROW:move = move_right();
                break;
            case EKEY::DOWNARROW:move = move_down();
                break;
            case EKEY::UPARROW:move = move_up();
                break;
            case EKEY::PRINTABLE:
                switch (key.value) {
                    case 'w':move = move_up();
                        break;
                    case 'a':move = move_left();
                        break;
                    case 's':move = move_down();
                        break;
                    case 'd':move = move_right();
                        break;
                    case 'x':
                    case 'q':return score();
                    default:break;
                }
            default:break;
        }
        if (move) {
            put_random();
            m_io.print_board(m_board);
        }
        if (is_lost())
            return score();
    }
}

void game::put_random()
{
    std::uniform_int_distribution<std::mt19937::result_type> distr(1, m_range);
    for (size_t n = 0; n < m_numgen; ++n) {
        auto zero_board = m_board | std::views::filter(is_zero);
        auto it = take_random(zero_board, m_gen);
        if (it == std::ranges::end(zero_board))
            return;
        auto bit = distr(m_gen);
        *it = 1 << bit;
    }
}

std::vector<std::pair<int, std::string>> game::load_highscores()
{
    auto filename = "highscores.txt"; //TODO
    std::ifstream ifs(filename);
    if (!ifs.is_open())
        return {};
    std::vector<std::pair<int, std::string>> ret;
    int score;
    std::string name;
    while (ifs >> score >> name)
        ret.emplace_back(score, name);
    return ret;
}

void game::save_highscores(const std::vector<std::pair<int, std::string>>& high_scores)
{
    auto filename = "highscores.txt"; //TODO
    std::ofstream ofs(filename);
    if (!ofs.is_open())
        return;
    for (auto& [score, name]: high_scores)
        ofs << score << ' ' << name << '\n';
}

void game::play_game()
{
    int score = run();
    std::string name = m_io.handle_highscore(m_board, score);
    std::vector<std::pair<int, std::string>> high_scores = load_highscores();
    high_scores.push_back({score, name});
    std::ranges::sort(high_scores, std::greater {});
    if (high_scores.size() > 10)
        high_scores.resize(10); //top 10 ?
    save_highscores(high_scores);
}


void game::main_loop()
{
    main_menu_.loop(exit_);
}

void game::show_highscores()
{
    m_io.print_highscores(load_highscores());
    m_io.keypress();
}

void game::options()
{
    //menu options_menu;
    m_io.clear_screen();
    std::cout << "This feature is not implemented yet.";
    m_io.keypress();
}

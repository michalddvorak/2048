#include "game.hpp"
#include <algorithm>
#include <ranges>
#include <fstream>

#include "../utils/utils.hpp"
#include "../utils/matrix_views.hpp"

game::game(size_t rows, size_t cols, size_t range, size_t numgen, unsigned int seed, io& io) :
        main_menu_(io, {
                           {constant("Play"),        [&]() { play_game(); }},
                           {constant("High-Scores"), [&]() { show_highscores(); }},
                           {constant("Options"),     [&]() { options(); }},
                           {constant("Exit"),        [&]() { exit_ = true; }}
                   },
                   R"(
  ___     ___    _  _     ___
 |__ \   / _ \  | || |   / _ \
    ) | | | | | | || |_ | (_) |
   / /  | | | | |__   _| | _ |
  / /_  | |_| |    | |  | (_) |
 |____|  \___/     |_|   \___/
)"),
        board_(rows, cols),
        m_range(range),
        m_numgen(numgen),
        m_gen(seed),
        m_io(io) { }


game::score_t game::run()
{
    board_.reset();
    m_io.clear_screen();
    board_.put_random_tiles(m_range, m_numgen, m_gen);
    m_io.print_board(board_);
    while (true) {
        bool move = false;
        auto key = m_io.get_key();
        switch (key.type) {
            case EKEY::LEFTARROW: move = board_.move_left();
                break;
            case EKEY::RIGHTARROW:move = board_.move_right();
                break;
            case EKEY::DOWNARROW:move = board_.move_down();
                break;
            case EKEY::UPARROW:move = board_.move_up();
                break;
            case EKEY::PRINTABLE:
                switch (key.value) {
                    case 'w':move = board_.move_up();
                        break;
                    case 'a':move = board_.move_left();
                        break;
                    case 's':move = board_.move_down();
                        break;
                    case 'd':move = board_.move_right();
                        break;
                    case 'x':
                    case 'q':return board_.score();
                    default:break;
                }
            default:break;
        }
        if (move) {
            board_.put_random_tiles(m_range, m_numgen, m_gen);
            m_io.print_board(board_);
        }
        if (board_.is_lost())
            return board_.score();
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
    std::string name = m_io.handle_highscore(board_, score);
    std::vector<std::pair<int, std::string>> high_scores = load_highscores();
    high_scores.push_back({score, name});
    std::ranges::sort(high_scores, std::greater {});
    /*if (high_scores.size() > 10)
        high_scores.resize(10); //top 10 ?*/
    save_highscores(high_scores);
}


void game::main_loop()
{
    main_menu_.loop(exit_);
}

void game::show_highscores()
{
    m_io.clear_screen();
    m_io.print_highscores(load_highscores());
    m_io.print_str("Press any key to return back to the main menu.");
    m_io.keypress();
}

void game::options()
{
    bool exit = false;
    using namespace std::string_literals;
    menu options_menu(m_io,
                      {{[&]() { return "Number of generated tiles: "s + std::to_string(m_numgen); }, [&]()
                                                                                                     {
                                                                                                         m_io.clear_screen();
                                                                                                         m_io.print_str(
                                                                                                                 "Enter new value for number of generated tiles (current = "s +
                                                                                                                 std::to_string(
                                                                                                                         m_numgen) +
                                                                                                                 "): "s);
                                                                                                         auto new_value = parse<size_t>(
                                                                                                                 m_io.get_string_from_user());
                                                                                                         m_io.clear_screen();
                                                                                                         if (!new_value)
                                                                                                             m_io.print_str(
                                                                                                                     "Could not parse number.");
                                                                                                         else {
                                                                                                             m_numgen = *new_value;
                                                                                                             m_io.print_str(
                                                                                                                     "New value for generated tiles: "s +
                                                                                                                     std::to_string(
                                                                                                                             *new_value));
                                                                                                         }
                                                                                                         m_io.print_str(
                                                                                                                 "\nPress any key to continue.");
                                                                                                         m_io.keypress();
                                                                                                     }},
                              //TODO
                       {[&]() { return "Range of generated tiles: "s + std::to_string(m_range); },   []() { }},
                       {constant(
                               "Back"),                                                              [&]() { exit = true; }}});
    options_menu.loop(exit);
}

#include "game.hpp"
#include <algorithm>
#include <ranges>
#include <fstream>

#include "../utils/utils.hpp"
#include "../utils/matrix_views.hpp"

game::game(size_t num_rows, size_t num_cols, size_t generated_tiles_range, size_t number_of_generated_tiles,
           unsigned int rng_seed, io& io, std::string highscore_filename) :
        highscore_(std::move(highscore_filename)),
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
        board_(num_rows, num_cols),
        generated_tiles_range_(generated_tiles_range),
        number_of_generated_tiles_(number_of_generated_tiles),
        gen_(rng_seed),
        io_(io) { }


game::score_t game::run_game()
{
    board_.reset();
    io_.clear_screen();
    board_.put_random_tiles(generated_tiles_range_, number_of_generated_tiles_, gen_);
    io_.print_board(board_);
    while (true) {
        bool move = false;
        auto key = io_.get_key();
        switch (key.type) {
            case EKEY::LEFTARROW:move = board_.move_left();
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
            board_.put_random_tiles(generated_tiles_range_, number_of_generated_tiles_, gen_);
            io_.print_board(board_);
        }
        if (board_.is_lost())
            return board_.score();
    }
}


void game::play_game()
{
    auto player_score = run_game();
    std::string player_name = io_.handle_highscore(board_, player_score);
    auto high_scores = highscore_.load_highscores();
    high_scores.push_back({player_score, player_name});
    std::ranges::sort(high_scores, std::greater {});
    /*if (high_scores.size() > 10)
        high_scores.resize(10); //top 10 ?*/
    highscore_.save_highscores(high_scores);
}


void game::main_loop()
{
    main_menu_.loop(exit_);
}

void game::show_highscores()
{
    io_.clear_screen();
    io_.print_highscores(highscore_.load_highscores());
    io_.print_str("Press any key to return back to the main menu.");
    io_.keypress();
}

void game::options()
{
    bool exit = false;
    using namespace std::string_literals;
    
    //TODO: split
    
    auto number_of_generated_tiles_show_callback = [&]()
    {
        return "Number of generated tiles: "s + std::to_string(number_of_generated_tiles_);
    };
    auto number_of_generated_tiles_action_callback = [&]()
    {
        io_.clear_screen();
        io_.print_str("Enter new value for number of generated tiles (current = "s +
                      std::to_string(number_of_generated_tiles_) + "): "s);
        auto new_value = parse<size_t>(
                io_.get_string_from_user());
        io_.clear_screen();
        if (!new_value)
            io_.print_str("Could not parse number.");
        else {
            number_of_generated_tiles_ = *new_value;
            io_.print_str("New value for generated tiles: "s + std::to_string(*new_value));
        }
        io_.print_str(
                "\nPress any key to continue.");
        io_.keypress();
    };
    
    auto number_of_generated_tiles_menu_item = menu_item {
            number_of_generated_tiles_show_callback,
            number_of_generated_tiles_action_callback
    };
    
    
    auto range_of_generated_tiles_show_callback = [&]()
    {
        return "Range of generated tiles: "s + std::to_string(generated_tiles_range_);
    };
    
    auto range_of_generated_tiles_action_callback = [&]()
    {
        io_.clear_screen();
        io_.print_str("This feature is not implemented yet. Press any key to continue.");
        io_.keypress();
    };
    
    auto range_of_generated_tiles_menu_item = menu_item {
            range_of_generated_tiles_show_callback,
            range_of_generated_tiles_action_callback
    };
    
    
    auto back_show_callback = constant("Back");
    auto back_action_callback = [&]() { exit = true; };
    auto back_menu_item = menu_item {
            back_show_callback,
            back_action_callback
    };
    
    
    menu<modulo_counter_circular<size_t>> options_menu(io_,
                                                       {
                                                               number_of_generated_tiles_menu_item,
                                                               range_of_generated_tiles_menu_item,
                                                               back_menu_item
                                                       },
                                                       R"(
     ____   _____   _______  _____   ____   _   _   _____
    / __ \ |  __ \ |__   __||_   _| / __ \ | \ | | / ____|
   | |  | || |__) |   | |     | |  | |  | ||  \| || (___
   | |  | ||  ___/    | |     | |  | |  | || . ` | \___ \
   | |__| || |        | |    _| |_ | |__| || |\  | ____) |
    \____/ |_|        |_|   |_____| \____/ |_| \_||_____/
)"
    );
    options_menu.loop(exit);
}


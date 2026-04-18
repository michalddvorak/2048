#pragma once

#include <cstddef>
#include <random>
#include <iostream>
#include "../utils/matrix.hpp"
#include "../io/io.hpp"
#include "menu.hpp"
#include "board.hpp"
#include "highscore.hpp"

class game
{
  public:
    using score_t = int;
    
    game(size_t num_rows,
         size_t num_cols,
         size_t generated_tiles_range,
         size_t number_of_generated_tiles,
         unsigned int rng_seed,
         io& io,
         std::string highscore_filename);
    
    void main_loop();
    
    void play_game();
    
    void show_highscores();
    
    void options();
  
  private:
    score_t run_game();
    highscore<score_t> highscore_;
    bool exit_ = false;
    menu<modulo_counter_circular<size_t>> main_menu_;
    board<int> board_;
    size_t generated_tiles_range_;
    size_t number_of_generated_tiles_;
    std::mt19937 gen_;
    io& io_;
};

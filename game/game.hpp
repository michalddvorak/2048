#pragma once

#include <cstddef>
#include <random>
#include <iostream>
#include "../utils/matrix.hpp"
#include "../io/io.hpp"
#include "Menu.hpp"

class game
{
  public:
    using score_t = int;
    
    game(size_t rows, size_t cols, size_t range, size_t numgen, io& io);
    
    void main_loop();
    
    void play_game();
    
    void show_highscores();
    
    void options();
  
  private:
    score_t run();
    
    std::vector<std::pair<int, std::string>> load_highscores();
    
    void save_highscores(const std::vector<std::pair<int, std::string>>& high_scores);
    
    void put_random();
    
    int score() const;
    
    bool move_left();
    
    bool move_right();
    
    bool move_down();
    
    bool move_up();
    
    bool is_lost() const;
    
    //TODO separate board :)
    
    bool exit_ = false;
    menu main_menu_;
    
    matrix<int> m_board;
    size_t m_range;
    size_t m_numgen;
    std::mt19937 m_gen = std::mt19937(std::random_device {}());
    io& m_io;
};

#pragma once

#include "../utils/matrix.hpp"
#include "../utils/ring.hpp"

enum class EKEY : int
{
    UPARROW,
    DOWNARROW,
    LEFTARROW,
    RIGHTARROW,
    ENTER,
    PRINTABLE,
    UNKNOWN
};

struct key
{
    EKEY type;
    char value;
};


class io
{
  public:
    virtual ~io() = default;
    
    virtual void clear_screen() = 0;
    
    virtual void print_menu(const ring<std::string>& menu) = 0;
    
    virtual void print_board(const matrix<int>& board) = 0;
    
    virtual std::string handle_highscore(const matrix<int>& board, int score) = 0;
    
    virtual void print_highscores(const std::vector<std::pair<int, std::string>>& high_scores) = 0;
    
    virtual void exit(const matrix<int>& board, int score) = 0;
    
    virtual key get_key() = 0;
    
    virtual void keypress() = 0;
};


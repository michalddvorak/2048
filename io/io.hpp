#pragma once

#include "../utils/matrix.hpp"

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
    
    virtual void print_menu(const std::vector<std::string>& menu, size_t selected) = 0;
    
    virtual void print_board(const matrix<int>& board) = 0;
    
    virtual std::string get_string_from_user() = 0;
    
    virtual std::string handle_highscore(const matrix<int>& board, int score) = 0;
    
    virtual void print_highscores(const std::vector<std::pair<int, std::string>>& high_scores) = 0;
    
    virtual void print_str(const std::string& str) = 0;
    
    virtual void exit(const matrix<int>& board, int score) = 0;
    
    virtual key get_key() = 0;
    
    virtual void keypress() = 0;
};


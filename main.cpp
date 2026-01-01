#include <iostream>
#include "game/game.hpp"
#include "io/termio.hpp"

//todo arg parsing (option file)
//todo menu
//todo high score
int main(int argc, char* argv[])
{
    term_io io(5, 5);
    game g(4, 4, 2, 1, std::random_device {}(), io);
    //Hmm.. this 5,5 of io should be carried as an argument to a print fn of io (perhaps it could be ignored by the implementation)
    //but in terminal it should not be ignored....??
    g.main_loop();
    return 0;
}

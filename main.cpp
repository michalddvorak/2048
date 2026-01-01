#include <iostream>
#include "game/game.hpp"
#include "io/termio.hpp"

//todo arg parsing (option file)
//todo menu
//todo high score
int main()
{
	term_io io(5,5);
	game g(4, 4, 2, 1, io);
	//g.run();
    g.main_loop();
	return 0;
}

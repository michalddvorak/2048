cmake_minimum_required(VERSION 3.20)
project(2048)

set(CMAKE_CXX_STANDARD 17)

add_executable(2048 main.cpp io/io.hpp io/termio.cpp io/termio.hpp utils/matrix.hpp utils/matrix_iterator.hpp game/game.cpp game/game.hpp utils/utils.hpp)
add_executable(test test.cpp utils/matrix.hpp utils/matrix_iterator.hpp utils/utils.hpp)
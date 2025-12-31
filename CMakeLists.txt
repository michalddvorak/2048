cmake_minimum_required(VERSION 3.20)
project(2048)

set(CMAKE_CXX_FLAGS -fconcepts-diagnostics-depth=10)
set(CMAKE_CXX_STANDARD 20)

add_executable(2048 main.cpp io/io.hpp io/termio.cpp io/termio.hpp utils/matrix.hpp game/game.cpp game/game.hpp utils/utils.hpp
        utils/matrix_views.hpp
        utils/strided_iterator.hpp)
add_executable(test test.cpp utils/matrix.hpp utils/utils.hpp
        utils/matrix_views.hpp
        utils/strided_iterator.hpp)

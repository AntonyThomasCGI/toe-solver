cmake_minimum_required(VERSION 3.21)
project(toe_solver VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_subdirectory(external/raylib)


include_directories(
    external/raylib
)


add_executable(${PROJECT_NAME}
    src/board.cpp
    src/board.hpp
    src/cell.cpp
    src/cell.hpp
    src/datatypes.hpp
    src/main.cpp
    src/player.cpp
    src/player.hpp
    src/solvers/abstract.hpp
    src/solvers/play_close.hpp
    src/solvers/random_move.hpp
)

target_link_libraries(${PROJECT_NAME} raylib)


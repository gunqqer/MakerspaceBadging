cmake_minimum_required(VERSION 3.10)

project(MakerspaceBadging VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)


set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -pedantic -Werror -D_GLIBCXX_DEBUG -lasn -fsanitize=undefined -fsanitize=address")
set(CMAKE_LINKER_FLAGS_DEBUG "-fsanitize=address")

#enable_testing()

file(GLOB_RECURSE sources ./src/*.cpp ./src/*.hpp)


add_executable(MakerspaceBadging ${sources})


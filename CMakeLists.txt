cmake_minimum_required(VERSION 3.15)
project(Chocopy-Compiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

add_executable(compiler src/main.cc src/Scanner.cc src/Mtoken.cc src/MessageGenerator.cc src/Parser.cc)

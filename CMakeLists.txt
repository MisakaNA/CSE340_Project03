cmake_minimum_required(VERSION 3.15)
project(Program)

set(CMAKE_CXX_STANDARD 11)

add_executable(Program compiler.cc compiler.h inputbuf.cc inputbuf.h lexer.cc lexer.h)
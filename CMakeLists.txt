cmake_minimum_required(VERSION 3.14)
project(veb_tree C)

set(CMAKE_C_STANDARD 99)

add_executable(veb_tree main.c veb_tree.h veb_tree.c dictionary.c dictionary.h)
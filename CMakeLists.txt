cmake_minimum_required(VERSION 3.0)

project(imzip LANGUAGES C)

add_executable(imzip main.c)

install(TARGETS imzip RUNTIME DESTINATION bin)

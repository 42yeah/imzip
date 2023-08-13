cmake_minimum_required(VERSION 3.0)

project(imzip LANGUAGES CXX)

add_executable(imzip main.cpp image.cpp)
target_include_directories(imzip PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS imzip RUNTIME DESTINATION bin)

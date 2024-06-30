cmake_minimum_required(VERSION 3.15)

project(Xeno LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_executable(xeno src/xeno.cpp src/film.cpp ext/lodepng.cpp)
cmake_minimum_required(VERSION 3.15)

project(Xeno LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(BUILD_SHARED_LIBS off)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(ext/glog)

#
#set_target_properties(glog PROPERTIES 
#    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/glog/lib 
#    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/glog/lib
#    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/glog/bin
#    )

add_executable(xeno src/xeno.cpp src/film.cpp ext/lodepng.cpp)

target_link_libraries(xeno glog::glog)
target_include_directories(xeno PRIVATE ext/glog/src)
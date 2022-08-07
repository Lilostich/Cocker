cmake_minimum_required(VERSION 3.20)
project(Maniso)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Maniso
        main.cpp creat_cgroup.h)
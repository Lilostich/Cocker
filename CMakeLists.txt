cmake_minimum_required(VERSION 3.20)
project(Cocker)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Cocker
        main.cpp creat_cgroup.h)
cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(user_engagement_stats)
add_executable(user_enegagement_stats user_engagement_stats.cpp Student.cpp split.cpp Video_stats.cpp)

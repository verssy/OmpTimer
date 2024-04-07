cmake_minimum_required(VERSION 3.24)

project(OmpTimer VERSION 0.1 LANGUAGES CXX)

find_package(OpenMP REQUIRED)

add_executable(OmpTest main.cpp)
set_target_properties(OmpTest PROPERTIES CXX_STANDARD 17)
target_link_directories(OmpTest PUBLIC OpenMP::OpenMP_CXX)

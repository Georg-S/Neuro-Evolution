cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
project(NeuroEvolution)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(NeuroEvolution)
add_subdirectory(Tests)
#add_subdirectory(2DEngine)
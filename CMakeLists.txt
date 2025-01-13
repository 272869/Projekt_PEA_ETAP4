cmake_minimum_required(VERSION 3.28)
project(Projekt_4)

set(CMAKE_CXX_STANDARD 17)

add_executable(Projekt_4 main.cpp
        GeneticAlgorithm.cpp
        ConfigFile.h
        ConfigFile.cpp
        FileReader.h
        FileReader.cpp)

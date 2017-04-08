cmake_minimum_required(VERSION 3.6)
project(ai)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Bot.cpp
        Bot.h
        Clamp.cpp
        Clamp.h
        SensorsBoard.cpp
        SensorsBoard.h)

add_executable(ai ${SOURCE_FILES})
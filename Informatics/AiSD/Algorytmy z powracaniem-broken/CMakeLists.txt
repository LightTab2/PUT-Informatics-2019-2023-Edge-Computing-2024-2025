cmake_minimum_required(VERSION 3.15)
project(Grafy)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        Grafy ${PROJECT_SOURCE_DIR}/source/Grafy.cpp
        ${PROJECT_SOURCE_DIR}/source/ZmienneGlobalne.h source/Wyswietlanie.h source/tworzeniemacierzy_list.h source/euler.h source/Hamilton.h)
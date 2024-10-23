cmake_minimum_required(VERSION 3.15)
project(Drzewa)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        Drzewa ${PROJECT_SOURCE_DIR}/source/Drzewa.cpp
        ${PROJECT_SOURCE_DIR}/source/Drzewa.h
        ${PROJECT_SOURCE_DIR}/source/Program.h
        ${PROJECT_SOURCE_DIR}/source/RotacjeAVL.h
        ${PROJECT_SOURCE_DIR}/source/WyswietlanieAVL.h
        ${PROJECT_SOURCE_DIR}/source/WyswietlanieBST.h
        ${PROJECT_SOURCE_DIR}/source/ModyfikowanieAVL.h
        ${PROJECT_SOURCE_DIR}/source/ModyfikowanieBST.h
        ${PROJECT_SOURCE_DIR}/source/Sortowania.h)
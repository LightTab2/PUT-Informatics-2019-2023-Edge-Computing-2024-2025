cmake_minimum_required(VERSION 3.15)
project(Grafy)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        Grafy ${PROJECT_SOURCE_DIR}/source/Grafy.cpp
        ${PROJECT_SOURCE_DIR}/source/DFS.h
        ${PROJECT_SOURCE_DIR}/source/SortowanieTopologiczne.h
        ${PROJECT_SOURCE_DIR}/source/ZmienneGlobalne.h source/macierzGrafu.h source/Wyswietlanie.h source/Wyswietlanie.h source/macierzSasiedztwa.h source/tworzeniemacierzy_list.h source/listanastepnikow.h)
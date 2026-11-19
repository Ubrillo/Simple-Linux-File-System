cmake_minimum_required(VERSION 3.28)
project(FileManager)

set(CMAKE_CXX_STANDARD 11)

add_executable(FileManager main.cpp
        node.h
        file.h
        folder.h
        file_manager.h
        folder.h
        file_system.h
        shell.cpp
        shell.h
)

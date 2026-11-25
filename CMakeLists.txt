cmake_minimum_required(VERSION 3.28)
project(FileManager)

set(CMAKE_CXX_STANDARD 11)

# The program is the interactive shell; its main() lives in shell.cpp.
# Only ONE .cpp in a target may define main(), so main.cpp (the old
# scratch test) is not listed here.
add_executable(
        FileManager
        shell.cpp
        shell.h
        node.h
        file.h
        folder.h
        file_manager.h
        file_system.h
)

target_compile_options(FileManager PRIVATE -Wall -Wextra)
cmake_minimum_required(VERSION 3.29)
project(Assembler C)

set(CMAKE_C_STANDARD 11)

add_executable(Assembler main.c
        file.c
        pre_proc.c
)

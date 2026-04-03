cmake_minimum_required(VERSION 4.2)
project(GridQualiyReport C)

set(CMAKE_C_STANDARD 99)

add_executable(GridQualiyReport main.c
        io.h
        waveform.h
        io.c
        waveform.c)

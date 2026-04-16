cmake_minimum_required(VERSION 4.2)
project(thread_pool_os_simulator C)

set(CMAKE_C_STANDARD 11)

add_executable(thread_pool_os_simulator Code/P1_00_Main.c Code/P1_01_Functions.c)

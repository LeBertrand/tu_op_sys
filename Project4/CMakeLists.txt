cmake_minimum_required(VERSION 3.9)
project(Project4)

set(CMAKE_CXX_STANDARD 11)

add_executable(Project4
        Testing/2mbmapping.c
        Testing/allocate_driver.c
        Testing/Drive2MB
        Testing/getpagesize.c
        Testing/wipeDrive.c
        boot_process.cpp
        DirectoryListObject.cpp
        DirectoryListObject.hh
        Drive2MB
        SOFAT.h
        SOFAT.hh
        SOFAT_allocate_block.c)

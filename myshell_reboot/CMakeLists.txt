cmake_minimum_required(VERSION 3.9)
project(myshell_reboot)

set(CMAKE_CXX_STANDARD 11)

add_executable(myshell_reboot
        reboot_tests.c
        reboot_framework.c)


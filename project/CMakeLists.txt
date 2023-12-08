cmake_minimum_required(VERSION 3.21)
project(project)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_executable(project main.cpp
        src/VendingMachine.cpp
        include/VendingMachine.h
        include/VendingMachine_events.h
        include/VendingMachine_states.h
        include/product.h)


cmake_minimum_required(VERSION 3.28)
project(Project2_DA)

set(CMAKE_CXX_STANDARD 17)

add_executable(Project2_DA main.cpp
        MutablePriorityQueue.h
        Graph.h
        Graph.h
        Interface.cpp
        Interface.h
        Printer.h
        DeliveryManager.cpp
        DeliveryManager.h
        Parser.cpp
        Parser.h
        Haversine.cpp
        Haversine.h)

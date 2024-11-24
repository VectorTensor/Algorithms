cmake_minimum_required(VERSION 3.27)
project(Algorithms)

set(CMAKE_CXX_STANDARD 14)

#add_library(Peer ./Services/AsioPeer.cpp)

add_executable(Algorithms
    ./Services/AsioPeer.cpp)

target_include_directories(Algorithms PRIVATE ./dependencies/asio-1.30.2/include)


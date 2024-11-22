cmake_minimum_required(VERSION 3.30)
project(Algorithms)

set(CMAKE_CXX_STANDARD 14)

include_directories(./dependencies/asio-1.30.2/include)
add_library(Peer ./Services/AsioPeer.cpp)

add_executable(Algorithms
    Main.cpp)
target_link_libraries(Algorithms Peer)

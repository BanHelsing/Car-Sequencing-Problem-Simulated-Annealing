cmake_minimum_required(VERSION 3.10)

# Define the project name
project(AIProject)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable optimization level O3
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Add the executable target
add_executable(main main.cpp)

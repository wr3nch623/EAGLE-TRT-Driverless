cmake_minimum_required(VERSION 3.10)  # Required CMake version
project(Driverless VERSION 0.1)  # Define project

set(CMAKE_CXX_STANDARD 17)  # Use C++17
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add an executable target (like "g++ main.cpp utils.cpp -o Driverless")
add_executable(Driverless
    ./main.cpp
)


cmake_minimum_required(VERSION 3.10)
project(Driverless VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Driverless 
    main.cpp
    include/WindowManager/windowmanager.cpp
    include/BicycleController/bicyclecontroller.cpp

)

target_include_directories(Driverless PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/RayLib
    ${CMAKE_SOURCE_DIR}/lib/WindowManager
)

# Directly link the .a file
target_link_libraries(Driverless PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/libraylib.a
    m pthread dl GL rt X11
)

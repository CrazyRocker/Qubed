cmake_minimum_required(VERSION 3.10)

# Project Name
project(Qubed CXX )

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Debug build
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Find Vulkan and GLFW
find_package(Vulkan 1.3 REQUIRED)
find_package(glfw3 3.4 REQUIRED)

# Source files
set(SOURCES src/main.cpp)

# Include directories
include_directories(${Vulkan_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})

# Define executable
add_executable(Qubed ${SOURCES})

# Link libraries
target_link_libraries(Qubed glfw Vulkan::Vulkan)

# Message output
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Vulkan Found: ${Vulkan_FOUND}")
message(STATUS "GLFW Found: ${GLFW_FOUND}")
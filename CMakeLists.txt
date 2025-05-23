cmake_minimum_required(VERSION 3.10)

# Project Name
project(Qubed CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Debug build
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Finding the Vulkan SDK
find_package(Vulkan 1.3 REQUIRED)

# Checking if the GLFW submodule exists, and getting GLFW from the submodule

set(GLFW_SUBMODULE_DIR "${CMAKE_SOURCE_DIR}/dependencies/glfw")

if(NOT EXISTS "${GLFW_SUBMODULE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "GLFW submodule not found! Please run:
    git submodule update --init --recursive
    while in the root of the repo!")
endif()

# Add GLFW
add_subdirectory(${GLFW_SUBMODULE_DIR})

# Source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)

# Include directories
include_directories(${Vulkan_INCLUDE_DIRS})

# Define executable
add_executable(Qubed ${SOURCES})

# Link libraries
target_link_libraries(Qubed glfw Vulkan::Vulkan)

# Message output
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Vulkan Found: ${Vulkan_FOUND}")
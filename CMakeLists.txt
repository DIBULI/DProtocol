cmake_minimum_required(VERSION 3.22)

# Set the project name
set(CMAKE_PROJECT_NAME dprotocol)
# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_CXX_STANDARD 11)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

FILE(GLOB_RECURSE DPROTOCOL_SOURCE_FILES 
  src/**.cpp
)

add_library(${CMAKE_PROJECT_NAME} INTERFACE)

target_sources(${CMAKE_PROJECT_NAME} INTERFACE ${DPROTOCOL_SOURCE_FILES})

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE
    # Add user defined include paths
    include
)



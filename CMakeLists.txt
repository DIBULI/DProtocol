cmake_minimum_required(VERSION 3.22)

enable_testing()

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm" OR CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")
        message(STATUS "Compiling for ARM microcontroller")
        set(MICROCONTROLLER_COMPILE TRUE)
    endif()
endif()


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

add_library(${CMAKE_PROJECT_NAME} INTERFACE)

if(MICROCONTROLLER_COMPILE)
  FILE(GLOB_RECURSE DPROTOCOL_SOURCE_FILES 
    src/circular_byte_array_freeRTOS.cpp
    src/d_protocol.cpp
  )
  # Add include paths
  target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE
    # Add user defined include paths
    include/common
    include/freeRTOS
  )
else()
  FILE(GLOB_RECURSE DPROTOCOL_SOURCE_FILES 
    src/circular_byte_array.cpp
    src/d_protocol.cpp
  )
  # Add include paths
  target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE
    # Add user defined include paths
    include/common
    include/std
  )
endif()


target_sources(${CMAKE_PROJECT_NAME} INTERFACE ${DPROTOCOL_SOURCE_FILES})

if (MICROCONTROLLER_COMPILE)
  target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE freertos_kernel)
endif()

if(NOT MICROCONTROLLER_COMPILE)
  add_executable(circular_byte_array_test tests/test_circular_byte_array.cpp)
  target_link_libraries(circular_byte_array_test ${CMAKE_PROJECT_NAME})
  add_test(NAME TestCircularByteArray
          COMMAND circular_byte_array_test)
endif()
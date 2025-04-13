cmake_minimum_required(VERSION 3.20)
project(fsm LANGUAGES CXX)

# Set C++ standard and compiler flags
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# Collect all source files (.cpp and .s) recursively from ./src
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.s"
)

# Include directories
file(GLOB_RECURSE INC_DIRS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*")
foreach(dir ${INC_DIRS})
    if(IS_DIRECTORY ${dir})
        include_directories(${dir})
    endif()
endforeach()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

# Add executable
add_executable(fsm ${SRC_FILES})

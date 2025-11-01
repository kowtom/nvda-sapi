cmake_minimum_required(VERSION 3.15)
project(nvda-sapi VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build both 32-bit and 64-bit versions
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_SUFFIX "64")
    message(STATUS "Building for x64")
else()
    set(ARCH_SUFFIX "32")
    message(STATUS "Building for x86")
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add source directory
add_subdirectory(src)

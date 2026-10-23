cmake_minimum_required(VERSION 3.10)

# Simple Hound App for Wii U - Minimal dependencies
project(HoundAppWiiUSimple CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Wii U toolchain
set(DEVKITPRO "$ENV{DEVKITPRO}" CACHE STRING "devkitPro path")
set(WUT_ROOT "$ENV{WUT_ROOT}" CACHE STRING "WUT path")

# Manually specify paths
set(SDL2_INCLUDE_DIR "$ENV{DEVKITPRO}/portlibs/wiiu/include/SDL2")
set(SDL2_LIBRARY "$ENV{DEVKITPRO}/portlibs/wiiu/lib/libSDL2.a")

# Include directories
include_directories(
    ${SDL2_INCLUDE_DIR}
    include
)

# Link directories
link_directories(
    $ENV{DEVKITPRO}/portlibs/wiiu/lib
    $ENV{DEVKITPRO}/wut/lib
)

# Source files
file(GLOB SOURCES "src/*.cpp" "src/*.c")
file(GLOB HEADERS "include/*.h" "include/*.hpp")

# Executable
add_executable(hound_app_wiiu ${SOURCES} ${HEADERS})

# Link libraries - only SDL2 and wut
target_link_libraries(hound_app_wiiu
    ${SDL2_LIBRARY}
    wut
)

# Wii U specific settings
set_target_properties(hound_app_wiiu PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output
    SUFFIX ".rpx"
)

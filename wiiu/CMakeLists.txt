cmake_minimum_required(VERSION 3.10)

# Project configuration
project(HoundAppWiiU CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages - use devkitPro's portlibs
list(APPEND CMAKE_PREFIX_PATH "$ENV{DEVKITPRO}/portlibs/wiiu")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_net REQUIRED)

# Include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_NET_INCLUDE_DIRS}
    include
)

# Link directories
link_directories(
    ${SDL2_LIBRARY_DIRS}
    ${SDL2_IMAGE_LIBRARY_DIRS}
    ${SDL2_TTF_LIBRARY_DIRS}
    ${SDL2_NET_LIBRARY_DIRS}
    $ENV{DEVKITPRO}/portlibs/wiiu/lib
)

# Source files
file(GLOB SOURCES "src/*.cpp" "src/*.c")
file(GLOB HEADERS "include/*.h" "include/*.hpp")

# Executable
add_executable(hound_app_wiiu ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(hound_app_wiiu
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_NET_LIBRARIES}
)

# Wii U specific settings
set_target_properties(hound_app_wiiu PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output
    SUFFIX ".rpx"
)

# Install target
install(TARGETS hound_app_wiiu
    RUNTIME DESTINATION bin
)

cmake_minimum_required(VERSION 3.10)

# Project configuration
project(HoundAppWiiU CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages - use devkitPro's portlibs
list(APPEND CMAKE_PREFIX_PATH "$ENV{DEVKITPRO}/portlibs/wiiu")

# Manually specify library paths for Wii U
set(SDL2_LIBRARIES "$ENV{DEVKITPRO}/portlibs/wiiu/lib/libSDL2.a")
set(SDL2_IMAGE_LIBRARIES "$ENV{DEVKITPRO}/portlibs/wiiu/lib/libSDL2_image.a")
set(SDL2_TTF_LIBRARIES "$ENV{DEVKITPRO}/portlibs/wiiu/lib/libSDL2_ttf.a")
set(SDL2_NET_LIBRARIES "$ENV{DEVKITPRO}/portlibs/wiiu/lib/libSDL2_net.a")

# Include directories
include_directories(
    $ENV{DEVKITPRO}/portlibs/wiiu/include
    $ENV{DEVKITPRO}/portlibs/wiiu/include/SDL2
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

# Link libraries
target_link_libraries(hound_app_wiiu
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_NET_LIBRARIES}
    wut
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

#!/bin/bash

# Simple Hound App Wii U Build Script
# Only requires SDL2 and wut - no image/ttf dependencies

set -e

# Configuration
PROJECT_NAME="hound_app_wiiu"
BUILD_DIR="build"
OUTPUT_DIR="output"

# Check for required environment variables
echo "Checking environment..."

if [ -z "$DEVKITPRO" ]; then
    echo "ERROR: DEVKITPRO environment variable not set"
    echo "Please run: export DEVKITPRO=/opt/devkitpro"
    echo "Trying with default /opt/devkitpro..."
    export DEVKITPRO=/opt/devkitpro
fi

if [ -z "$WUT_ROOT" ]; then
    echo "WARNING: WUT_ROOT environment variable not set"
    echo "Setting WUT_ROOT=$DEVKITPRO/wut"
    export WUT_ROOT=$DEVKITPRO/wut
fi

echo "DEVKITPRO: $DEVKITPRO"
echo "WUT_ROOT: $WUT_ROOT"

# Check for compiler
CMAKE_CMD=""
if [ -f "$DEVKITPRO/portlibs/wiiu/bin/powerpc-eabi-cmake" ]; then
    CMAKE_CMD="$DEVKITPRO/portlibs/wiiu/bin/powerpc-eabi-cmake"
    echo "Using CMake: $CMAKE_CMD"
elif [ -f "$DEVKITPRO/wut/bin/powerpc-eabi-cmake" ]; then
    CMAKE_CMD="$DEVKITPRO/wut/bin/powerpc-eabi-cmake"
    echo "Using CMake: $CMAKE_CMD"
else
    echo "ERROR: powerpc-eabi-cmake not found"
    echo "Please install wiiu-dev package"
    exit 1
fi

# Create build directory
rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# Set CMake prefix path
CMAKE_PREFIX_PATH="$DEVKITPRO/portlibs/wiiu"
export CMAKE_PREFIX_PATH

# Run CMake
$CMAKE_CMD .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH

# Build the project
echo "Building..."
make -j$(nproc)

# Create output directory
mkdir -p ../$OUTPUT_DIR

# Copy the RPX to output
if [ -f $PROJECT_NAME.rpx ]; then
    cp $PROJECT_NAME.rpx ../$OUTPUT_DIR/
    echo ""
    echo "✅ Build successful!"
    echo "RPX file: ../$OUTPUT_DIR/$PROJECT_NAME.rpx"
    echo ""
    echo "To install on Wii U:"
    echo "  1. Copy $PROJECT_NAME.rpx to sd:/wiiu/apps/hound_app/"
    echo "  2. Copy assets/meta.xml to the same directory"
    echo "  3. Launch from Homebrew Launcher"
else
    echo "ERROR: Build failed - RPX file not found"
    exit 1
fi

cd ..

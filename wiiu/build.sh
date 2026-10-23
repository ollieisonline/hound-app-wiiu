#!/bin/bash

# Hound App Wii U Build Script
# This script builds the Wii U version of Hound App using devkitPro and wut

set -e

# Configuration
PROJECT_NAME="hound_app_wiiu"
BUILD_DIR="build"
OUTPUT_DIR="output"

# Check for required environment variables
if [ -z "$DEVKITPRO" ]; then
    echo "ERROR: DEVKITPRO environment variable not set"
    echo "Please set DEVKITPRO to your devkitPro installation path"
    exit 1
fi

if [ -z "$WUT_ROOT" ]; then
    echo "ERROR: WUT_ROOT environment variable not set"
    echo "Please set WUT_ROOT to your wut installation path"
    exit 1
fi

# Create build directory
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# Run CMake
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=$WUT_ROOT/share/wut.toolchain.cmake \
    -DCMAKE_INSTALL_PREFIX=$DEVKITPRO/portlibs/wiiu \
    -G "Unix Makefiles"

# Build the project
make -j$(nproc)

# Create output directory
mkdir -p ../$OUTPUT_DIR

# Copy the RPX to output
if [ -f $PROJECT_NAME.rpx ]; then
    cp $PROJECT_NAME.rpx ../$OUTPUT_DIR/
    echo "Build successful! RPX file: ../$OUTPUT_DIR/$PROJECT_NAME.rpx"
else
    echo "ERROR: Build failed - RPX file not found"
    exit 1
fi

cd ..

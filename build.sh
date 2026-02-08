#!/bin/bash

# --------------------------------------
# Build script for CMake project
# --------------------------------------

# Exit immediately if a command fails
set -e

# Build directory
BUILD_DIR=build

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

# Go into the build directory
cd "$BUILD_DIR"

# Run CMake to configure the build system
# You can change "Unix Makefiles" to "Ninja" if you prefer
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build .

# Go back to the project root
cd ..

echo "Build finished successfully!"

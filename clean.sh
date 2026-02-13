#!/bin/bash

# --------------------------------------
# Clean the build folder
# --------------------------------------

# Exit on error
set -e

BUILD_DIR=build

if [ -d "$BUILD_DIR" ]; then
    echo "Deleting $BUILD_DIR folder..."
    rm -rf "$BUILD_DIR"
    echo "Build folder deleted."
else
    echo "No build folder found."
fi

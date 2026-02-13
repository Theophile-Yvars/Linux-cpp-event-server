#!/bin/bash

# --------------------------------------
# Run the EventServerMain executable
# --------------------------------------

# Exit on error
set -e

# Path to the build folder
BUILD_DIR=build

# Check if executable exists
EXEC="$BUILD_DIR/EventServerMain"

if [ ! -f "$EXEC" ]; then
    echo "Executable not found. Please build first!"
    exit 1
fi

# Run the executable
echo "Running EventServerMain..."
"$EXEC"

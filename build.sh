#!/bin/bash

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
  mkdir build
fi

# Navigate to build directory
cd build

# Run CMake and make
cmake ..
make

# Return to the original directory
cd ..

echo "Build completed."

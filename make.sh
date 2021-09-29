#!/bin/bash

# Create documentation.
#mkdir -p doc
#cd doc
#doxygen
#cd ..

# Compile.
cmake -B build -DCMAKE_BUILD_TYPE=Debug .
cmake --build build

# Start program.
./build/maybe-a-game

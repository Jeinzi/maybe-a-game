#!/bin/bash

#cd doc
#doxygen
#cd ../bin
cmake -B build -DCMAKE_BUILD_TYPE=Release .
cmake --build build
./build/flowerpower
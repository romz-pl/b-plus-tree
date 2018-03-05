#!/bin/bash

#
# Create 'build' directory
#
rm -rf build || exit 1
mkdir build || exit 1
cd build || exit 1

#
# Run cmake
#
cmake -DBUILD_GTEST=ON -DBUILD_GMOCK=OFF -DCMAKE_BUILD_TYPE=Debug  .. || exit 1
make || exit 1

# make -j8 extendible-hashing_coverage

#
# Run test
#
#ctest --verbose




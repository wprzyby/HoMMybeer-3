#!/bin/bash

apt install -y gcc cmake ninja
cmake --preset="main-release" .
cmake --build ./build/main-release
ln -s ./build/main-release/_deps/sfml-build/lib .
./build/main-release/bin/HoMMybeer3
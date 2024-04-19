#!/bin/bash

cmake --preset="main-release" . 
cmake --build ./build/main-release
./build/main-release/bin/HoMMybeer3

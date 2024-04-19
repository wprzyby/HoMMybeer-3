#!/bin/bash

apt install -y gcc cmake ninja-build git xorg-dev libgl1-mesa-dev libudev-dev libalut-dev libvorbis-dev libflac-dev
cmake --preset="main-release" . 
cmake --build ./build/main-release
./build/main-release/bin/HoMMybeer3
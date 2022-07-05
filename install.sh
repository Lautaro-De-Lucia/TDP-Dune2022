#!/bin/bash

echo "==========================================================="
echo "=================== Dune 2022 Installer ==================="
echo "==========================================================="

# dependencies
sudo apt install cmake
sudo apt install libsdl2-dev
sudo apt install libsdl2-ttf-dev
sudo apt install libsdl2-image-dev
sudo apt install qt5-default
sudo apt install qtbase5-dev
sudo apt install qtmultimedia5-dev
sudo apt install gstreamer1.0-plugins-good
sudo apt install libqt5multimedia5-plugins

# compiling
mkdir build
cd build
cmake -S ../ -B .
make all

echo "==========================================================="
echo "================== Installation complete =================="
echo "==========================================================="

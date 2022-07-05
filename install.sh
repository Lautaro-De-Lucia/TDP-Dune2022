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

# copying data contents to /var/dune2022
cd ..
cd data
sudo mkdir -p /var/dune2022
sudo cp -r * /var/dune2022

# copying binaries to /usr/bin
cd ..
cd build
chmod +x dune2022server
chmod +x dune2022client
sudo cp dune2022server /usr/bin
sudo cp dune2022client /usr/bin

echo "==========================================================="
echo "================== Installation complete =================="
echo "==========================================================="
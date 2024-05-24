#! /bin/bash

# File: build.sh
# Project: arduino-resource-monitor
# Created Date: 2024-05-24 10:15:30
# Author: 3urobeat
#
# Last Modified: 2024-05-24 13:03:15
# Modified By: 3urobeat
#
# Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
#
# This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
# You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.


# Clean-Builds all platforms for release


# Get system cores (50% of what make could use for '-j' because hyperthreading but this will do, this server does not take long to compile)
SYS_CORES=$( grep -c ^processor /proc/cpuinfo )

echo "System Cores: $SYS_CORES"

# Get version
VERSION=$( grep "#define version" server.h | awk '{split($0,a," "); print $3}' | sed 's/"//g' )

echo "Building Version: $VERSION"

# Create build dir if it does not exist yet. We cannot cd into it because dockcross must be executed from the src directory
mkdir -p ./build

# Exit if docker is not installed
( docker stats --no-stream > /dev/null ) || { echo "Error: Docker not installed or docker daemon not running! Exiting..." ; exit 1; }



# ------- x86_64 -------
echo "x86_64: Processing..."

rm -rf ./build/build-x86_64
mkdir ./build/build-x86_64


# Get dockcross binary if it does not exist yet
if [ ! -f ./build/dockcross/dockcross-manylinux-x64 ]; then
    echo "x86_64: Docker image 'dockcross-manylinux-x64' does not exist yet. Pulling..."

    docker pull dockcross/manylinux-x64 || { echo "x86_64: Pull failed! Exiting..."; exit 1; }

    docker run --rm dockcross/manylinux-x64 > ./build/dockcross/dockcross-manylinux-x64
    chmod +x ./dockcross/dockcross-manylinux-x64
fi


# Compile and rename release mode
echo "x86_64: Compiling release..."

./build/dockcross/dockcross-manylinux-x64 bash -c "cd ./build/build-x86_64 && cmake -DBUILD_RELEASE=ON ../.. && make -j$SYS_CORES" || { echo "x86_64: Failed to compile release! Exiting..."; exit 1; }

cp ./build/build-x86_64/arduino-resource-monitor-server-linux ./build/arduino-resource-monitor-server-$VERSION-x86_64-linux
chmod +x ./build/arduino-resource-monitor-server-$VERSION-x86_64-linux


# Compile and rename release + clientLessMode mode
echo "x86_64: Compiling release + clientLessMode..."

rm ./build/build-x86_64/CMakeCache.txt # Sadly required for cmake to use our -DBUILD_RELEASE_CLIENT_LESS flag

./build/dockcross/dockcross-manylinux-x64 bash -c "cd ./build/build-x86_64 && cmake -DBUILD_RELEASE_CLIENT_LESS=ON ../.. && make -j$SYS_CORES" || { echo "x86_64: Failed to compile release + clientLessMode! Exiting..."; exit 1; }

cp ./build/build-x86_64/arduino-resource-monitor-server-linux ./build/arduino-resource-monitor-server-$VERSION-x86_64-linux-clientLessMode
chmod +x ./build/arduino-resource-monitor-server-$VERSION-x86_64-linux-clientLessMode


# Exit
echo "x86_64: Done!"



# ------- armv7l -------
echo "armv7l: Processing..."

rm -rf ./build/build-armv7l
mkdir ./build/build-armv7l


# Get dockcross binary if it does not exist yet
if [ ! -f ./build/dockcross/dockcross-linux-armv6 ]; then
    echo "armv7l: Docker image 'dockcross-linux-armv6' does not exist yet. Pulling..."

    docker pull dockcross/linux-armv6:20200324-880bfd0 || { echo "armv7l: Pull failed! Exiting..."; exit 1; }

    docker run --rm dockcross/linux-armv6:20200324-880bfd0 > ./build/dockcross/dockcross-linux-armv6
    chmod +x ./dockcross/dockcross-linux-armv6
fi


# Compile and rename release mode
echo "armv7l: Compiling release..."

./build/dockcross/dockcross-linux-armv6 bash -c "cd ./build/build-armv7l && cmake -DBUILD_RELEASE=ON ../.. && make -j$SYS_CORES" || { echo "armv7l: Failed to compile release! Exiting..."; exit 1; }

cp ./build/build-armv7l/arduino-resource-monitor-server-linux ./build/arduino-resource-monitor-server-$VERSION-armv7l-linux
chmod +x ./build/arduino-resource-monitor-server-$VERSION-armv7l-linux


# Compile and rename release + clientLessMode mode
echo "armv7l: Compiling release + clientLessMode..."

rm ./build/build-armv7l/CMakeCache.txt # Sadly required for cmake to use our -DBUILD_RELEASE_CLIENT_LESS flag

./build/dockcross/dockcross-linux-armv6 bash -c "cd ./build/build-armv7l && cmake -DBUILD_RELEASE_CLIENT_LESS=ON ../.. && make -j$SYS_CORES" || { echo "armv7l: Failed to compile release + clientLessMode! Exiting..."; exit 1; }

cp ./build/build-armv7l/arduino-resource-monitor-server-linux ./build/arduino-resource-monitor-server-$VERSION-armv7l-linux-clientLessMode
chmod +x ./build/arduino-resource-monitor-server-$VERSION-armv7l-linux-clientLessMode


# Exit
echo "armv7l: Done!"
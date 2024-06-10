#! /bin/bash

# File: build-releases.sh
# Project: arduino-resource-monitor
# Created Date: 2024-05-24 10:15:30
# Author: 3urobeat
#
# Last Modified: 2024-06-10 18:04:34
# Modified By: 3urobeat
#
# Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
#
# This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
# You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.


# Clean-Builds all platforms for release


# Get version
VERSION=$( grep "#define version" src/main.h | awk '{split($0,a," "); print $3}' | sed 's/"//g' )

echo "-> Building Version: $VERSION"

# Exit if platformio is not installed
( platformio --version > /dev/null ) || { echo "-> Error: PlatformIO is not installed! Exiting..." ; exit 1; }

# Create build dir if it does not exist yet
mkdir -p ./build

# Remove old firmwares
rm ./build/arduino-resource-monitor-client-*


# ------- nano -------
echo "-> nano: Processing..."


# Clean
echo "-> nano: Cleaning..."

platformio run --target clean --environment nano


# Build and rename
echo "-> nano: Compiling release..."

platformio run --environment nano || { echo "-> nano: Failed to compile release! Exiting..."; exit 1; }

cp ./.pio/build/nano/firmware.hex ./build/arduino-resource-monitor-client-$VERSION-nano.hex


# Exit
echo "-> nano: Done!"



# ------- nanoNew -------
echo "-> nanoNew: Processing..."


# Clean
echo "-> nanoNew: Cleaning..."

platformio run --target clean --environment nanoNew


# Build and rename
echo "-> nanoNew: Compiling release..."

platformio run --environment nanoNew || { echo "-> nanoNew: Failed to compile release! Exiting..."; exit 1; }

cp ./.pio/build/nanoNew/firmware.hex ./build/arduino-resource-monitor-client-$VERSION-nanoNew.hex


# Exit
echo "-> nanoNew: Done!"

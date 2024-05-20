# Server for Linux
Reads resource usage data and sends it to the arduino.

This server uses the [serial](https://github.com/wjwwood/serial) library by wjwwood to communicate with the Arduino over serial. I removed a few Windows & OSX files.  
This youtube tutorial explains using it quite well: https://youtu.be/uHw7QyL4CM8

&nbsp;

## Compiling
**Prerequisites:**  
We are using docker to link against an older version of glibc, which is required to support older Linux installations.  
Project used: https://github.com/dockcross/dockcross  
Related CMake E-Mail: https://cmake.org/pipermail/cmake/2017-February/064959.html  

```bash
# Pull image once
docker pull dockcross/manylinux-x64

# Create 'build' folder in this directory (if not already done)
mkdir build && cd build

# Get container 
docker run --rm dockcross/manylinux-x64 > dockcross-manylinux-x64
chmod +x ./dockcross-manylinux-x64
```

**Compile:**  
```bash
# Do the thing (run from this directory, *not* from build).
# You can increase '-j4' to how many threads your CPU has. More = Faster
./dockcross-manylinux-x64 bash -c "cd build && cmake .. && make -j4"

# Optional - See against which glibc version the binary has been compiled:
objdump -p ./build/arduino-resource-monitor-server-linux
```

**Run:**  
If the build succeeded, a binary called 'arduino-resource-monitor-server-linux' should have appeared in the 'build' directory.  

```bash
# It should already be executable. If it however isn't, run:
chmod +x ./build/arduino-resource-monitor-server-linux

# Execute it:
./build/arduino-resource-monitor-server-linux
```

**One-liner for development:**  
```bash
./dockcross-manylinux-x64 bash -c "cd build && cmake .. && make -j4" && ./build/arduino-resource-monitor-server-linux
```

# Server for Linux
[⬅️ Go back to the main page](../..#readme)

Reads the CPU, RAM & GPU usage of your Linux PC and sends it to the Arduino.  

This server uses the [C-Periphery](https://github.com/vsergeev/c-periphery) library. It is licensed under the [MIT](https://opensource.org/license/MIT) license.

&nbsp;

## Table of Contents
- [Download](#download)
- [Optional: Compiling yourself](#compiling)
- [Running](#running)
- [Configuration](#config)
- [Troubleshooting](#troubleshooting)

&nbsp;

<a id="download"></a>

## Download
Download the Linux Server from the latest [release](https://github.com/3urobeat/arduino-resource-monitor/releases/latest).  
It is located at the bottom in the 'Assets' section.  

> [!NOTE]
> The version of the server (your PC) and client (the Arduino) must match.  
> Should your Arduino run on an older version, then either update it or download an older version of the server.  
> You can find every version on the [releases](https://github.com/3urobeat/arduino-resource-monitor/releases) page.

You can copy the binary on your system to anywhere you like.

&nbsp;

<a id="compiling"></a>

### Optional: Compiling yourself (x86)
If you'd like to compile the server yourself instead of downloading it from the releases section, do this:

<details>
<summary>(Click to expand)</summary>
&nbsp;

**Prerequisites:**  
We are using docker to build for multiple architectures and to link against an older version of glibc, which is required to support older Linux installations.  
Project used: https://github.com/dockcross/dockcross  
Related CMake E-Mail: https://cmake.org/pipermail/cmake/2017-February/064959.html  

This explanation is to compile during development for x86, which your system probably is.  
See/Run the [build-releases.sh](./build-releases.sh) script to clean-build all platforms!

```bash
# Pull x86_64 image once
docker pull dockcross/manylinux-x64

# Create 'build' & 'dockcross' folders (if not already done)
mkdir -p build/build-x86_64
mkdir -p build/dockcross

# Get container 
docker run --rm dockcross/manylinux-x64 > ./build/dockcross/dockcross-manylinux-x64

chmod +x ./build/dockcross/dockcross-manylinux-x64
```

**Compile:**  
```bash
# Do the thing (run from this directory, *not* from build).
# You can increase '-j4' to how many threads your CPU has. More = Faster
./build/dockcross/dockcross-manylinux-x64 bash -c "cd build/build-x86_64 && cmake ../.. && make -j4"

# Optional - See against which glibc version the binary has been compiled:
objdump -p ./build/build-x86_64/arduino-resource-monitor-server-linux
```

**Run:**  
If the build succeeded, a binary called 'arduino-resource-monitor-server-linux' should have appeared in the 'build-x86_64' directory.  

```bash
# It should already be executable. If it however isn't, run:
chmod +x ./build/build-x86_64/arduino-resource-monitor-server-linux

# Execute it:
./build/build-x86_64/arduino-resource-monitor-server-linux
```

**One-liner for x86 development:**  
```bash
./build/dockcross/dockcross-manylinux-x64 bash -c "cd ./build/build-x86_64 && cmake ../.. && make -j4" && ./build/build-x86_64/arduino-resource-monitor-server-linux
```

> [!NOTE]
> It does make sense to compile locally during development and to only use dockcross before publishing.  
> Compile Errors will have broken links when compiling using dockcross, making it harder to jump to them quickly.

Use the build.sh script for compiling binaries meant for releasing instead!

</details>

&nbsp;

<a id="running"></a>

## Running
**Permissions:**  
Before running, make sure you can access your USB ports without requiring admin privileges.  
To check, connect the Arduino and run the terminal command: `cat /dev/ttyUSB0`  

If the command exits without any message, you can skip this and go directly to the 'Execute' chapter below.  
If the command returns a "Permission denied" error, you need to add yourself to a group.  

&nbsp;

Run `ls -al /dev/ttyUSB0` to see which group your distribution uses.  
On Arch based distros you should see **uucp**: `... root uucp ... /dev/ttyUSB0` (shortened)  
On Debian based distros it is probably **dialout**: `... root dialout ... /dev/ttyUSB0` (shortened)

Add yourself to the respective group. The variable `$USER` will be automatically replaced with your username.
- e.g. on Arch: `sudo usermod -aG uucp $USER`
- e.g. on Debian: `sudo usermod -aG dialout $USER`

&nbsp;

This change requires a relog to be applied.  
You can work around this until you relog by running the command `su $USER`.  
You need to execute the binary in the next step from the same terminal for this workaround to work!

&nbsp;

**Execute:**  
Open a terminal at the location of where you put the downloaded/compiled binary.

Make sure your binary is executable by running:  
`chmod +x ./arduino-resource-monitor-server-linux`  

Make sure the Arduino is connected to your computer and displays "Waiting...".  
Execute the binary by running:  
`./arduino-resource-monitor-server-linux &` 

The server will now start scanning your USB ports for the Arduino and attempt to connect to it.  
Once connected, it starts taking measurements and sends them to the Arduino.  

You can send the process into the background to be able to the close the terminal by running `disown`.  
(This requires the binary to have been started with `&` at the end, just like shown above)

&nbsp;

<a id="config"></a>

## Manual Configuration
TODO

&nbsp;

<a id="troubleshooting"></a>

## Troubleshooting
**Server does not find the Arduino**  
TODO

**Sensor data is missing/wrong**  
TODO

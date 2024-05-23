# Server for Linux
Reads the CPU, RAM & GPU usage of your Linux PC and sends it to the Arduino.  

This server uses the [C-Periphery](https://github.com/vsergeev/c-periphery) library. It is licensed under the [MIT](https://opensource.org/license/MIT) license.

&nbsp;

## Download
Download the Linux Server from the latest [release](https://github.com/3urobeat/arduino-resource-monitor/releases/latest).  
It is located at the bottom in the 'Assets' section.  

> [!NOTE]
> The version of the server (your PC) and client (the Arduino) must match.  
> Should your Arduino run on an older version, then either update it or download an older version of the server.  
> You can find every version on the [releases](https://github.com/3urobeat/arduino-resource-monitor/releases) page.

You can copy the binary on your system to anywhere you like.

&nbsp;

### Optional: Compiling yourself
If you'd like to compile the server yourself instead of downloading it from the releases section, do this:

<details>
<summary>(Click to expand)</summary>
&nbsp;

**Prerequisites:**  
We are using docker to build for multiple architectures and to link against an older version of glibc, which is required to support older Linux installations.  
Project used: https://github.com/dockcross/dockcross  
Related CMake E-Mail: https://cmake.org/pipermail/cmake/2017-February/064959.html  

```bash
# Pull x86_64 & armv7l images once
docker pull dockcross/manylinux-x64
docker pull dockcross/linux-armv6:20200324-880bfd0

# Create 'build' folder in this directory (if not already done)
mkdir build
mkdir build-armv7l

# Get container 
docker run --rm dockcross/manylinux-x64 > dockcross-manylinux-x64
docker run --rm dockcross/linux-armv6:20200324-880bfd0 > dockcross-linux-armv6

chmod +x ./dockcross-manylinux-x64
chmod +x ./dockcross-linux-armv6
```

**Compile:**  
```bash
# Do the thing (run from this directory, *not* from build).
# You can increase '-j4' to how many threads your CPU has. More = Faster
./dockcross-manylinux-x64 bash -c "cd build && cmake .. && make -j4"
./dockcross-linux-armv6 bash -c "cd build-armv7l && cmake .. && make -j4"

# Optional - See against which glibc version the binary has been compiled:
objdump -p ./build/arduino-resource-monitor-server-linux
objdump -p ./build-armv7l/arduino-resource-monitor-server-linux
```

**Run:**  
If the build succeeded, a binary called 'arduino-resource-monitor-server-linux' should have appeared in the 'build' directory.  

```bash
# It should already be executable. If it however isn't, run:
chmod +x ./build/arduino-resource-monitor-server-linux

# Execute it:
./build/arduino-resource-monitor-server-linux
```

Append `-x86_64` to the filename of the x86 binary and `-armv7l` to the armv7l binary.

**One-liner for x86 development:**  
```bash
./dockcross-manylinux-x64 bash -c "cd build && cmake .. && make -j4" && ./build/arduino-resource-monitor-server-linux
```

> [!NOTE]
> It does make sense to compile locally during development and to only use dockcross before publishing.  
> Compile Errors will have broken links when compiling using dockcross, making it harder to jump to them quickly.

</details>

&nbsp;

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

## Troubleshooting
**Server does not find the Arduino**  
TODO

**Sensor data is missing/wrong**  
TODO

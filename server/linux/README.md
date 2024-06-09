# Server for Linux
[⬅️ Go back to the main page](../..#readme)

Reads the CPU, RAM & GPU usage of your Linux PC and sends it to the Arduino.  
The utilization data is collected from the Linux Kernel `sysfs` & `procfs`.

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

If you have an Nvidia card, you also need to have `nvidia-settings` installed.

&nbsp;

<a id="compiling"></a>

### Optional: Compiling yourself (x86)
If you'd like to compile the server yourself instead of downloading it from the releases section, do this:

<details>
<summary>(Click to expand)</summary>
&nbsp;

**Are you a user?**  
Make sure you have `make` & `cmake` installed. Open a terminal in *this* (`server/linux/`) folder.

Then simply run:  
```bash
rm -rf ./build/build-x86_64 && mkdir -p ./build/build-x86_64 && cd build/build-x86_64 && cmake -DBUILD_RELEASE=ON ../.. && make -j4 && cp arduino-resource-monitor-server-linux ~/arduino-resource-monitor-server-linux ; cd ../..
```
**This command will:** Clean the build folder, Create a new build folder, Go into it, Compile the binary, Copy it into your home directory and Go back to the this directory.  

This is suitable when compiling the binary only for your own (this) machine. You can skip right ahead to [Running](#running)! 

&nbsp;

**Are you a maintainer/developer? Continue reading:**

**Prerequisites:**  
We are using docker to build for multiple architectures and to link against an older version of glibc, which is required to support older Linux installations.  
Project used: https://github.com/dockcross/dockcross  
Related CMake E-Mail: https://cmake.org/pipermail/cmake/2017-February/064959.html  

This explanation is to compile during development for x86, which your system probably is.  
See/Run the [build-releases.sh](./build-releases.sh) script to clean-build all platforms!

```bash
# Pull x86_64 image once
docker pull dockcross/manylinux-x64

# Clean build folder and create folders (if not already done)
rm -rf ./build/build-x86_64     # It is important to clean up to make sure cmake does not use flags from a previous built
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
# Clean up once before compiling the first time during your session to remove CMakeCache containing flags from previous attempts
rm -rf ./build/build-x86_64 && mkdir -p ./build/build-x86_64

# Build and run repeatedly using
cd build/build-x86_64 && cmake ../.. && make -j4 ; cd ../.. && ./build/build-x86_64/arduino-resource-monitor-server-linux
```

> [!IMPORTANT]
> Use the one-liner during development - Compile Errors will have broken links when compiling using dockcross, making it harder to jump to them quickly.
> When done, **use the `build-releases.sh` script to compile all binaries meant to be released!**

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

&nbsp;

Do you have a **NVIDIA** GPU? **Stop!**  
Stop the server with <kbd>CTRL</kbd>+<kbd>C</kbd> and read [Manual Configuration](#config) below.  
You need to set your `gpuType` to "nvidia" before proceeding. 

AMD users can ignore this part, as it is the default setting.  
You have an Intel card? Feel free to open an issue and help me to add support for it.

&nbsp;

The server will now start scanning your USB ports for the Arduino and attempt to connect to it.  
Once connected, it starts taking measurements and sends them to the Arduino.  

You can send the process into the background to be able to the close the terminal by running `disown`.  
(This requires the binary to have been started with `&` at the end, just like shown above)

You have multiple AMD GPUs and the wrong one has been chosen? Please read [Troubleshooting](#troubleshooting).

&nbsp;

<a id="config"></a>

## Manual Configuration
The server creates a configuration file after the first start, located at `~/.config/arduino-resource-monitor/config.toml`.  
This file allows you to customize connection timeouts, your GPU type and sensors.  

See the sensor finding help below at [Troubleshooting](#troubleshooting) for more information.

**This table contains all available settings:**  

| Key | Type | Description |
| --- | ---- | ----------- |
| createdWithVersion | string | Stores the version the config file was created with. <br> **Don't modify.** |
| | &nbsp; |
| arduinoReplyTimeout | int | Time in milliseconds the server will wait for a response from the Arduino when connecting. <br> Default: 5000 |
| connectionRetryTimeout | int | Base time (see multiplier below) in milliseconds the server will wait before retrying to connect. <br> Default: 5000 |
| connectionRetryAmount | int | The amount of times the server will attempt to reconnect before giving up and exiting. <br> Default: 10 |
| connectionRetryMultiplier | float | The amount by which `connectionRetryTimeout` is multiplied with on every reconnect attempt. <br> Default: 0.5 |
| | &nbsp; |
| gpuType | "amd" or "nvidia" | Type of GPU you use (I have no Intel GPU to test, try "amd" and feel free to open an issue). <br> AMD will attempt to find a sysfs hwmon sensor, NVIDIA will rely on readings from `nvidia-settings` (make sure you have it installed). <br> Default: "amd" |
| cpuTempSensorPath | string | Path to a sysfs HwMon or ThermalZone file that should override the default CPU Temperature search path. <br> Search for `HwMon CPU Temp` in [getSensors.c](src/sensors/getSensors.c) to see the default search terms. <br> Default: "" (empty string to not override default) |
| gpuLoadSensorPath | string | Path to a sysfs HwMon or ThermalZone file that should override the default GPU Load search path. <br> Search for `HwMon GPU Load & Temp` in [getSensors.c](src/sensors/getSensors.c) to see the default search terms. <br> Make sure to keep `gpuType` at default. <br> Default: "" (empty string to not override default) |
| gpuTempSensorPath | string | Path to a sysfs HwMon or ThermalZone file that should override the default GPU Temperature search path. <br> Search for `HwMon GPU Load & Temp` in [getSensors.c](src/sensors/getSensors.c) to see the default search terms. <br> Make sure to keep `gpuType` at default. <br> Default: "" (empty string to not override default) |
| checkInterval | int | Time in milliseconds the server will wait between taking + sending measurements. Minimum is 1000. <br> Default: 1000 |


&nbsp;

<a id="troubleshooting"></a>

## Troubleshooting
**Sensor is missing/data is wrong/I have multiple GPUs and the wrong one is used!**  
<details>
<summary>CPU Load, RAM Usage or Swap Usage: (Click to expand)</summary>
&nbsp;

If CPU Load, RAM usage or Swap usage is wrong, please open an [Issue](https://github.com/3urobeat/arduino-resource-monitor/issues/new).  
Unless you are using a very old Linux kernel, these measurements should be correct across all devices because they use the same "sensor", directly implemented by the kernel.  
If your Swap usage shows "/", your system does not have a Swap partition/file configured (like mine in the demo image).

</details>

<br>

<details>
<summary>NVIDIA GPU: (Click to expand)</summary>
&nbsp;

If you are using an Nvidia card, have `gpuType` set to "nvidia", have `nvidia-settings` installed and the data is still wrong or you are getting errors, please open an [Issue](https://github.com/3urobeat/arduino-resource-monitor/issues/new).  
I'm not sure which GPU is used by default when you have multiple Nvidia GPUs.

</details> 

<br>

<details>
<summary>CPU Temp, AMD GPU Load or AMD GPU Temp: (Click to expand)</summary>
&nbsp;

Your system might be using an uncommon sensor name.  
Run the command `cat /sys/class/hwmon/hwmon*/name` and see if you spot a familiar name.  
Do you see duplicates? You might have two GPUs (I do) and the wrong one was chosen.  

The command `ls -al /sys/class/hwmon/hwmon*` displays where these sensor names link to.  
You can find the IDs listed in this output again in the output of `lspci` (requires package `pciutils` to be installed), together with a device name.  
This should help you identify which sensor belongs to which device.

Some devices also might not have a sensor! Two of my older Intel Thinkpads for example do not have any sensors for their iGPUs.

Follow the example below.

<br>

**Example for hwmon name:**  
AMD usually uses `k10temp` for their CPUs and `amdgpu` for their GPUs. Intel usually uses `coretemp` for their CPUs. Raspberry PIs usually use `cpu_thermal`.  
These are the names the server looks for by default.  
On my system the `cat /sys/class/hwmon/hwmon*/name` command returns:  
```
nvme
k10temp
amdgpu
amdgpu
hidpp_battery_0
```

> **I found one of those names but the server did not/chose the wrong one:**  
> Let's use the named strategy from above with `lspci` first to find the correct amdgpu of the two.  
> For me `lspci` contains:  
> `03:00.0 VGA compatible controller: Advanced Micro Devices, Inc. [AMD/ATI] Navi 31 [Radeon RX 7900 XT/7900 XTX/7900M] (rev cc)`  
> I see the ID `03:00.0` again in `ls -al /sys/class/hwmon/hwmon*` (look at the last part before '/hwmon/'):  
> `lrwxrwxrwx 1 root root 0  9. Jun 08:39 /sys/class/hwmon/hwmon2 -> ../../devices/pci0000:00/0000:00:01.1/0000:01:00.0/0000:02:00.0/0000:03:00.0/hwmon/hwmon2`
>
> Let's check the contents of that directory `/sys/class/hwmon/hwmon2` to find the sensor for our configuration:  
> `ls /sys/class/hwmon/hwmon2/`  
>
> See a `temp1_input` sensor? Configure that path at `gpuTempSensorPath` in the config. In this case it would look like this:  
> `gpuTempSensorPath = "/sys/class/hwmon/hwmon2/temp1_input"`
>
> Look around for something that looks like a load/util/busy sensor. In my case the device has a subdirectory `device` with a bunch of more sensors.  
> Let's look inside: `ls /sys/class/hwmon/hwmon2/device/`  
> I see a `gpu_busy_percent` sensor! When probing it with `cat /sys/class/hwmon/hwmon2/device/gpu_busy_percent` it returns `6`; a valid looking value.  
> Config now has: `gpuLoadSensorPath = "/sys/class/hwmon/hwmon2/device/gpu_busy_percent"`
>
> Found a load sensor that returns values in a different format? Open an [Issue](https://github.com/3urobeat/arduino-resource-monitor/issues/new).


> **I found sensors with other names/the named strategy from above did not work for me:**  
> Get the name again of every sensor: `cat /sys/class/hwmon/hwmon*/name`  
> Get the content of all those sensors: `ls /sys/class/hwmon/hwmon*/`
>
> CPU:  
> Install the package `stress` and load up your CPU a bit: `stress -c 4`.  
> Leave this terminal open, open another one, continue reading and use it for running any commands below.
>
> GPU:  
> Start a game/4k Video to get a noticeable higher temperature on the right GPU.  
> 
> Check each hwmon (by reading the output of the 'ls' command from above) containing a `temp1_input` sensor for a fitting temperature.  
> In my case, when searching for the CPU, the device `hwmon1` contains a sensor with that name.  
> The command `cat /sys/class/hwmon/hwmon1/temp1_input` returns `70250` (== 70.25 °C); which looks good.  
> Config now contains: `cpuTempSensorPath = "/sys/class/hwmon/hwmon1/temp1_input"`

<br>

**No fitting hwmon found:**  
You can repeat the same/similar finding-sensors-by-increasing-temp steps for `/sys/devices/virtual/thermal/thermal_zone*`.  
I'm not going into detail as it should be pretty self explanatory when following the steps above.

These are usually only used on some ARM devices like Raspberry Pis or the Nvidia Jetson Nano.  
My x86 machines do not have any `thermal_zone` sensors in that directory.

<br>

My Jetson Nano had its GPU Load sensor at `/sys/devices/57000000.gpu/load`. You might wanna look into `/sys/devices/` as well.

<br>

If you could not find a fitting sensor (for example for an iGPU), your device might just not have one.  
Nothing you can do (as far as I know).

If you need further help, do not hesitate to open an [Issue](https://github.com/3urobeat/arduino-resource-monitor/issues/new).

<br>

**Done:**  
Did this help you find your sensor?  
Please open an [Issue](https://github.com/3urobeat/arduino-resource-monitor/issues/new) to help me add support for your sensor by default, making it easier for other users in the future.

</details>

&nbsp;

**Server does not find the Arduino!**  
This can have various reasons - you might have to search the internet yourself.  
All my devices supported the Arduino out of the box, except one where `/dev/ttyUSB*` did not show up.  

<details>
<summary>Nontheless, here are a few points to look into: (Click to expand)</summary>
&nbsp;

- Is the USB cable bad?
  - Try another *(seriously, this happens more often than you think - check this first)*
- Run the command `ls -al /dev/ttyUSB*` before and after connecting the Arduino. Does a new device show up?
  - [Searching the internet](https://duckduckgo.com/?t=ffab&hps=1&q=linux+arduino+%2Fdev%2FttyUSB+does+not+show+up&ia=web) indicates a package `brltty` might cause the issue (on Debian based distros).  
- Run the command `sudo dmesg` before and after connecting the Arduino. Do new messages show up?
  - If yes, search these errors on the internet. This might return a very helpful indication of what the cause is.
    - Even though very rare (the Linux Kernel supports most devices OOB) you might need to install a (different) driver.  
      See the product page of where you bought your Arduino.
  - If no, no connection was recognized. You might have a hardware related issue (USB port/USB cable/Arduino).
- You forgot to change permissions (although the server should still find devices and show an error instead).
  - Run the command `ls -al /dev/ttyUSB*` and look for the group required to access it.  
    Example with uucp (yours might say dialout or something else): `... root uucp ... /dev/ttyUSB0`  
    Make sure that group appears in the output of the command `id`. If not, read [Permissions](#running) again.  
    Start the server from this same terminal. 

</details>

&nbsp;

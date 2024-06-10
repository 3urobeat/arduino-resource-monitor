# Arduino Client
Receives CPU, RAM & GPU usage via USB (Serial) from the server and displays it on the connected LCD screen.

&nbsp;

## Table of Contents
- [Download](#download)
- [Optional: Compiling yourself](#compiling)
- [Flashing Firmware](#flashing)
- [Connecting Display](#connecting)

&nbsp;

<a id="download"></a>

## Download
Download the Arduino Firmware from the latest [release](https://github.com/3urobeat/arduino-resource-monitor/releases/latest).  
It is located at the bottom in the 'Assets' section.  

> [!NOTE]
> The version of the server (your PC) and client (the Arduino) must match.  
> Make sure to download the latest server when installing the latest firmware.  
> You can find every version on the [releases](https://github.com/3urobeat/arduino-resource-monitor/releases) page.

&nbsp;

<a id="compiling"></a>

### Optional: Compiling yourself
If you'd like to compile the client yourself instead of downloading it from the releases section, do this:

<details>
<summary>(Click to expand)</summary>
&nbsp;

**Preface:**  
This firmware was built using the help of PlatformIO for compiling, flashing and debugging.  
It's great, I totally recommend it to anyone.

Connect the Arduino.  
Make sure the correct path to the USB device is set in `platformio.ini` at `upload_port`.

<br>

**Method 1:** CLI, more suited for only installing 
Install PlatformIO Core through your package manager (`platformio-core` on Arch) or using a installer from their [website](https://platformio.org/install/cli).  
Open the `client` subdirectory (this one) in your terminal/PowerShell.

Depending on if you are using an Arduino with the old or the new bootloader, run one of these commands:  
Old: `platformio run --target upload --environment nano`  
New: `platformio run --target upload --environment nanoNew` 

<br>

**Method 2:** Code Editor, more suited for development  
Install VsCodium and the PlatformIO extension from the Extensions tab.  
Open the `client` subdirectory (this one) in VsCodium. PlatformIO should detect the configuration file inside it.  

Depending on if you are using an Arduino with the old or the new bootloader, choose the environment `nano` or `nanoNew` at the bottom of VsCodium.  
Hit the '->' button at the bottom to build and flash.

<br>

> [!IMPORTANT]
> Use these methods to compile only during development.  
> When done, **use the `build-releases.sh` script to compile all firmwares meant to be released!**  
> The build script requires `platformio` to be installed through your package manager.

</details>

&nbsp;

<a id="flashing"></a>

## Flashing the Firmware
TBA, there are no really great options out there right now, except PlatformIO or the official Arduino IDE, which are technically more aimed at developers.  

For now, please read the [Compiling yourself](#compiling) instructions from above.  
Instead of running the build command listed there, run:  
Old bootloader: `platformio run --target upload --environment nano`  
New bootloader: `platformio run --target upload --environment nanoNew`

After the upload has finished, you can uninstall PIO Core again.

&nbsp;

<a id="connecting"></a>

## Connecting the Display
Connect your display like this:  
`SDA` -> `A4`  
`SDC` -> `A5`  
`VCC` -> `5V` or `VIN`  
`GND` -> `GND`  

(left is the display side, right the Arduino side)

&nbsp;

### Optional: Connecting a Display Backlight switch
Connect your switch like this:  
`GND` -> Switch  
Switch -> `D2`

# Arduino Client
Receives CPU, RAM & GPU usage via USB (Serial) from the server and displays it on the connected LCD screen.

&nbsp;

## Download
Download the Arduino Firmware from the latest [release](https://github.com/3urobeat/arduino-resource-monitor/releases/latest).  
It is located at the bottom in the 'Assets' section.  

> [!NOTE]
> The version of the server (your PC) and client (the Arduino) must match.  
> Make sure to download the latest server when installing the latest firmware.  
> You can find every version on the [releases](https://github.com/3urobeat/arduino-resource-monitor/releases) page.

&nbsp;

### Optional: Compiling yourself
If you'd like to compile the client yourself instead of downloading it from the releases section, do this:

<details>
<summary>(Click to expand)</summary>
&nbsp;

Compile & Flash using Platform.IO

TODO

</details>

&nbsp;

## Flashing the Firmware
TODO

&nbsp;

## Connect the display to your Arduino
Connect your display like this:  
`SDA` -> `A4`  
`SDC` -> `A5`  
`VCC` -> `5V`  
`GND` -> `GND`  

(left is the display side, right the Arduino side)

&nbsp;

### Optional: Connect a Display Backlight switch
Connect your switch like this:  
`GND` -> Switch  
Switch -> `D2`

# Server for Windows  
[⬅️ Go back to the main page](../..#readme)

Reads the CPU, RAM & GPU usage of your Windows PC and sends it to the Arduino.  

This server uses the [LibreHardwareMonitor](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor) library. It is licensed under the [MPL 2.0](https://www.mozilla.org/en-US/MPL/2.0/) license.

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
Download the Windows Server from the latest [release](https://github.com/3urobeat/arduino-resource-monitor/releases/latest).  
It is located at the bottom in the 'Assets' section.  

> [!NOTE]
> The version of the server (your PC) and client (the Arduino) must match.  
> Should your Arduino run on an older version, then either update it or download an older version of the server.  
> You can find every version on the [releases](https://github.com/3urobeat/arduino-resource-monitor/releases) page.

You can copy the executable on your system to anywhere you like.

&nbsp;

<a id="compiling"></a>

### Optional: Compiling yourself (x86)
If you'd like to compile the server yourself instead of downloading it from the releases section, do this:

<details>
<summary>(Click to expand)</summary>
&nbsp;

**Are you a user?**  
Make sure you have [Dotnet SDK 6.0](https://dotnet.microsoft.com/en-us/download/dotnet/6.0) installed. Open a PowerShell window in *this* (`server/windows/`) folder.  
(You can do this by clicking the blue `File` button in the top left of the file explorer and clicking `Open Windows PowerShell`)

Then simply run:  
```bash 
Remove-Item -Path ".\bin" -Force -Recurse ; Remove-Item -Path ".\build" -Force -Recurse ; Remove-Item -Path ".\obj" -Force -Recurse ; mkdir .\build ; dotnet publish -c Release -r win-x64 ; copy .\bin\Release\net6.0\win-x64\publish\arduino-resource-monitor-server-windows.exe .\build\arduino-resource-monitor-server-windows.exe
```
<!-- Note: Windows PowerShell is stupid and does not support && but ;. Shit like -AND did not work for me. -->

**This command will:** Remove all three build folders, create new output folder, compile the exe and copy it into `.\build` with the right name.  
The 3 Remove-Item commands might show an error if the directories do not exist yet (which will be the case with a fresh repository clone). You can ignore them.

This is suitable when compiling the executable only for your own (this) machine. You can skip right ahead to [Running](#running)! 

&nbsp;

**Are you a maintainer/developer? Continue reading:**

**Prerequisites:**  
Install Visual Studio and dotnet.

This explanation is to compile during development for x86, which your system probably is.  
See/Run the [build-releases.bat](./build-releases.bat) script to clean-build all platforms!

**Compile from VS:**  
Open the solution file `src/server/windows/windows.sln` in Visual Studio.  
Right click on the solution "windows" in your solution explorer and click on Publish.

There should be a pre-configured configuration shown to you now.  
Hit Publish at the top and wait until finished.  

Your `.exe` is now located under `src/server/windows/bin/Release/net6.0/publish/win-x64/arduino-resource-monitor-server-windows.exe`. Rename and copy the .exe to anywhere you like.

&nbsp;

> [!IMPORTANT]
> Use VS to compile only during development.  
> When done, **use the `build-releases.bat` script to compile all executables meant to be released!**  
>
> Note: Dotnet sometimes likes to let building fail, showing red errors. Starting the script again always worked for me.

</details>

&nbsp;

<a id="running"></a>

## Running
Make sure the Arduino is connected to your computer and displays "Waiting...".  
Double click the `.exe`, accept the privilege escalation window and a CMD prompt should appear.

> [!NOTE]
> Why Admin?  
> The server needs to run with admin privileges to be able to access & read hardware sensors.  
> This is the same case with well known Hardware Monitoring programs like HwInfo.

The server will now start scanning your USB ports for the Arduino and attempt to connect to it.  
Once connected, it starts taking measurements and sends them to the Arduino.

When the CMD prompt is closed, the server will exit and the Arduino will display a "Lost Connection!" message after 5 seconds.

&nbsp;

<a id="config"></a>

## Manual Configuration
The server creates a configuration file after the first start, located at `C:\Users\%USERNAME%\AppData\Local\arduino-resource-monitor\config.ini`.  
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
| cpuHardwareName | string | Name of a CPU in your system that should be the only CPU to use sensors from. <br> You can find the name in the Task Manager. <br> Default: "" (empty string to not override default) |
| gpuHardwareName | string | Name of a GPU in your system that should be the only GPU to use sensors from. <br> You can find the name in the Task Manager. <br> Default: "" (empty string to not override default) |
| cpuLoadSensorName | string | Name of a CPU Load sensor that should override what the server automatically searches for. <br> Default: "" (empty string to not override default) |
| cpuTempSensorName | string | Name of a CPU Temperature sensor that should override what the server automatically searches for. <br> Default: "" (empty string to not override default) |
| gpuLoadSensorName | string | Name of a GPU Load sensor that should override what the server automatically searches for. <br> Default: "" (empty string to not override default) |
| gpuTempSensorName | string | Name of a GPU Temperature sensor that should override what the server automatically searches for. <br> Default: "" (empty string to not override default) |
| checkInterval | int | Time in milliseconds the server will wait between taking + sending measurements. Minimum is 1000. <br> Default: 1000 |

&nbsp;

<a id="troubleshooting"></a>

## Troubleshooting
**Sensor is missing/data is wrong/I have multiple GPUs and the wrong one is used!**  
Why does Swap always show "/"?  
As of now, the Windows Server does not display any value for it because Windows does swapping differently than Linux.  

<details>
<summary>Wrong device has been chosen: (Click to expand)</summary>
&nbsp;

If you have multiple GPUs for example, set `gpuHardwareName` in the config to the name of your GPU.  
In my case, where I have an iGPU and a dGPU in my system, I had to change the setting to:  
`gpuHardwareName = "AMD Radeon RX 7900 XT"`

You can find the name of your CPU and GPU in the Task Manager under the `Performance` tab.

</details>

<br>

<details>
<summary>The correct device but the wrong sensor has been chosen: (Click to expand)</summary>
&nbsp;

Download the `-printDebug` version from the [release section](https://github.com/3urobeat/arduino-resource-monitor/releases/latest).  
Upon running, it will log all devices, its sensors and their current value.

Look through them and find the correct sensor.  
Copy its name and set it in the config.

Example for a CPU Temperature sensor:  
`cpuTempSensorName = "Core (Tctl/Tdie)"`

</details>

&nbsp;

**Server does not find the Arduino!**  
You might need to install a USB driver for your board (I needed to install one for Win10 1803, but not for Win10 22H2).  
My distributor AzDelivery lists a CH340 driver on their [Store Page](https://www.az-delivery.de/en/products/nano-v3-mit-ch340-arduino-kompatibel).  
Check the product page of your seller, or try installing this [C340 Driver](https://cdn.shopify.com/s/files/1/1509/1638/files/ch340.zip?v=1683899825) as well.

&nbsp;

**Visual Studio does not find dependencies!**  
In my case the nuget package source address was missing. Follow [this guide](https://learn.microsoft.com/en-us/nuget/consume-packages/install-use-packages-visual-studio#package-sources) to add it.

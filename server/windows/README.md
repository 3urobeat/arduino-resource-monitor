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

### Optional: Compiling yourself
If you'd like to compile the server yourself instead of downloading it from the releases section, do this:

<details>
<summary>(Click to expand)</summary>
&nbsp;

Open the solution file `src/server/windows/windows.sln` in Visual Studio.  
Right click on the solution "windows" in your solution explorer and click on Publish.  
There should be a pre-configured configuration shown to you now. Hit Publish at the top and wait until finished.  
Your `.exe` is now located under `src/server/windows/bin/Release/net6.0/publish/win-x64/arduino-resource-monitor-server-windows.exe`. Rename and copy the .exe to anywhere you like.

You can probably also compile this project without the full Visual Studio editor installed, but I don't know exactly how.
(I usually develop on Linux)

</details>

&nbsp;

<a id="running"></a>

## Running
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
TODO

&nbsp;

<a id="troubleshooting"></a>

## Troubleshooting
**Server does not find the Arduino**  
You might need to install a USB driver for your board (I needed to install one for Win10 1803, but not for Win10 22H2).  
My distributor AzDelivery lists a CH340 driver on their [Store Page](https://www.az-delivery.de/en/products/nano-v3-mit-ch340-arduino-kompatibel).  
Check the product page of your seller, or try installing this [C340 Driver](https://cdn.shopify.com/s/files/1/1509/1638/files/ch340.zip?v=1683899825) as well.

**Sensor data is missing/wrong**  
TODO

**Visual Studio does not find dependencies**  
In my case the nuget package source address was missing. Follow [this guide](https://learn.microsoft.com/en-us/nuget/consume-packages/install-use-packages-visual-studio#package-sources) to add it.

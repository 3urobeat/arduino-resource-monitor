<div align="center">
    <h1>arduino-resource-monitor</h1>
    <h4>📊 Display your computer's CPU, RAM & GPU usage on a LCD display!</h4>
    <div>
        <a href="#introduction">Introduction</a> •
        <a href="#support">Support</a> •
        <a href="#try">Try it out!</a> •
        <a href="#hardware">Hardware</a> •
        <a href="#setup">Setup</a>
    </div>
    <br>
    <div>
      <img width=48% height:auto src="https://raw.githubusercontent.com/3urobeat/arduino-resource-monitor/main/.github/img/display.jpg">  
      <img align="right" width=49% height:auto src="https://raw.githubusercontent.com/3urobeat/arduino-resource-monitor/main/.github/img/server.jpg">  
    </div>
</div>

&nbsp;

<a id="introduction"></a>

## ✨ Introduction
This project consists out of two parts, the **client** (Arduino) and the **server** (Application).

The server application, which runs on your PC, takes measurements of your CPU, RAM & GPU utilization every second.  
It then sends the collected data to the Arduino, which is connected to your PC via USB.  
The Arduino receives the data and updates the screen.

Using this project, you always have your PC stats at a quick glance, even when gaming.  
Monitor your PC without needing to tab out, open another application or using another screen altogether!

**Linux x86_64 (64-bit)**, **Linux armv6 (ARM 32-bit)**, **Linux aarch64 (ARM 64-bit)** and **Windows x86_64 (64-bit)** are supported.  

&nbsp;

&nbsp;

<a id="support"></a>

## 💻️ PC Hardware & OS Support
I have successfully tested the project with:
- **Linux x86_64:**
  - **Arch 6.8.9-arch1-2:** Desktop - AMD Ryzen 9 7900X, AMD Radeon RX 7900XT
  - **Arch 6.9.1-arch1-2:** Thinkpad Z13 - AMD Ryzen 5 PRO 6650U with Radeon 660M
  - **Arch 6.8.5-arch1-1:** Thinkpad T430 - Intel i5 3320M *(No iGPU hwmon sensor)*
  - **Mint 4.15.0-20-generic:** Acer Aspire 5250 - AMD E-300 *(No iGPU hwmon sensor)*
  - **Debian 5.15.74-1-pve:** Desktop/Server - AMD Ryzen 5 2600
  - **Kali 6.5.0-kali3-amd64:** ***!*** *Virtual Machine* ***!*** - AMD Ryzen 9 7900X *(CPU Temperature not supported)*

- **Linux armv6:**
  - **Debian 6.1.21-v7+:** Raspberry Pi 3 Model B Plus Rev 1.3 - ARMv7 Processor rev 4 (v7l) 
  - **Debian 6.1.21+:** Raspberry Pi Zero W Rev 1.1 - ARMv6-compatible processor rev 7 (v6l)

- **Linux aarch64:**
  - **Debian 4.9.253-tegra:** NVIDIA Jetson Nano Developer Kit - ARMv8 Processor rev 1 (v8l) *(Configure GPU Load manually: `/sys/devices/57000000.gpu/load`)*

- **Windows x86_64:** 
  - **Windows 10 22H2:** Desktop - AMD Ryzen 9 7900X, AMD Radeon RX 7900XT
  - **Windows 10 22H2:** ***!*** *Virtual Machine* ***!*** - AMD Ryzen 9 7900X *(CPU Temperature not supported)*
  - **Windows 10 1803:** Desktop - Intel i7 4790, Nvidia RTX 2060
  - **Windows 10 22H2:** Desktop - Intel i7 4790, Nvidia RTX 2060

&nbsp;

<a id="try"></a>

### ⚠️ Wanna try it now on your machine before proceeding any further?
Download the latest `-clientLessMode` executable for your OS from the [release section](https://github.com/3urobeat/arduino-resource-monitor/releases/latest) and run it!  
It will display the measured data directly on your screen, allowing you to see if it works on your machine before buying an Arduino!

Please read the 'Running' section for your OS first:
- [Linux](server/linux#running)
- [Windows](server/windows#running)

It explains relevant information about required permissions for reading sensor data.

&nbsp;

<a id="hardware"></a>

## 🎛️ Client Hardware
- Arduino Nano ([Amazon](https://www.amazon.com/s?k=arduino+nano))  
  I recommend buying one with already soldered pins. This allows you to connect it easily.
- LCD Display 4 rows x 20 columns ([Amazon](https://www.amazon.com/s?k=lcd+display+2004))  
  They are available in Green & Blue. You need one with the 'Serial Adapter', preferably pre-soldered if you can find one.
- Breadboard Jumper Cables ([Amazon](https://www.amazon.com/s?k=breadboard+jumper+wires+female+to+female))  
  You need 4 cables to connect the display to the Arduino.
- Optional: Backlight Switch ([Amazon](https://www.amazon.com/s?k=breadboard+mini+toggle+slide+switch))  
  This allows you to toggle on/off the display backlight. You need 2 more jumper cables!
- Optional: USB cable, if not delivered with the Arduino Nano
- Optional: Some sort of case to house the Arduino & Display in

Combined this should cost $20-$30.

&nbsp;

<a id="setup"></a>

## 🚀 Client Setup Instructions
Visit the client README to learn how to setup your Arduino.  
Click below:

**Arduino:**  
See [client/README.md](client/README.md)

&nbsp;

## 🚀 Server Setup Instructions
The setup instructions are well documented in the respective directory for your platform.  
Choose below:

**Linux:**  
See [server/linux/README.md](server/linux/README.md)

**Windows:**  
See [server/windows/README.md](server/windows/README.md)

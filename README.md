# arduino-resource-monitor
 
Display the resource usage of your PC on an 4x20 LCD display!  
This project retreives CPU, MEM & GPU usage and sends it via Serial to an Arduino Nano which is connected via USB.  

<div>
  <img width=45% height:auto src="https://raw.githubusercontent.com/HerrEurobeat/arduino-resource-monitor/main/.github/img/display.jpg">  
  <img align="right" width=45% height:auto src="https://raw.githubusercontent.com/HerrEurobeat/arduino-resource-monitor/main/.github/img/server.jpg">  
</div>

&nbsp;

## Installation
Install the following packages using your package manager:  
```
make  
cmake  
sysstat  
lm_sensors  
radeontop       (If you are using an AMD GPU)  
nvidia-settings (If you are using an NVIDIA GPU)
```
(I hope I didn't miss anything. The package names may deviate if you aren't on Arch)  

&nbsp;

## Configuration
**Linux:**  
Before compiling, open `src/server/linux/server.h` with a text editor.  
Run `sensors` in another terminal, find your CPU temp sensor and enter it at `cpuTempSensor`.  
  
Set `gpuType` to 0 if you have an NVIDIA GPU or to 1 if you have an AMD GPU.  
  
If you've got an AMD GPU:  
Find your GPU temp sensor in the readout from above as well and enter it at `gpuTempSensor`.  
Run `lspci | grep Radeon` and find the bus id of your GPU, it's the first number before the `:` of the line describing your card. My dedicated card has the ID `03`.  

If you've got multiple USB devices connected that use `/dev/ttyUSB` paths then you might need to change `port`. I'd recommend leaving it as default for now and changing it later should the connection not work.  

**Windows:**  
Not yet supported.  

&nbsp;

## Compiling the server
**Linux:**  
Go into the `build` folder of the linux server. Run these commands:  
`cmake ..`  
`make -j 6` (replace the 6 with how many CPU threads you want to use, more = faster)  

You now have a binary `arduino-resource-monitor-server-linux` which you can put anywhere on your system.  

**Windows:**  
Not yet supported.  

&nbsp;

## Compiling the client
Compile & Upload the `client` directory using `platformio`.  

Instructions TBA

&nbsp;

## Connecting the Display to your Arduino
Connect your display like this:  
`SDA` -> `A4`  
`SDC` -> `A5`  
`VCC` -> `5V`  
`GND` -> `GND`  


&nbsp;

## Running
Before running, make sure you are in the group uucp, otherwise you won't be able to execute the binary without root permissions:  
`sudo usermod -aG uucp $USER`  
You probably need to relog for this change to be applied. You can work around this until you relog by running `su $USER` in the terminal that you'll use to execute the binary in a few moments.  
  
Make sure your binary is executable by running:  
`sudo chmod +x ./arduino-resource-monitor-server-linux`  

Connect the Arduino Nano to your PC and wait for it to start.  
Now execute the binary by running `./arduino-resource-monitor-server-linux`. Your LCD display should start displaying values!  

You can send the process into the background to be able to the close the terminal by pressing <kbd>CTRL</kbd>+<kbd>Z</kbd>, running `bg` and then `disown`.
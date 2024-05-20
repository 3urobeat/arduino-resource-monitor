# arduino-resource-monitor
Display the resource usage of your PC on an 4x20 LCD display!  
This project retrieves CPU, RAM & GPU usage and sends it to an Arduino Nano which is connected via USB.  
Both Linux and Windows are supported.

<div>
  <img width=45% height:auto src="https://raw.githubusercontent.com/3urobeat/arduino-resource-monitor/main/.github/img/display.jpg">  
  <img align="right" width=45% height:auto src="https://raw.githubusercontent.com/3urobeat/arduino-resource-monitor/main/.github/img/server.jpg">  
</div>

&nbsp;

&nbsp;

## Configuration
Both servers have automatic USB port and sensor discovery.  
Should the application not find a sensor automatically, you can configure them manually.

**Linux:**  
TODO

**Windows:**  
TODO

&nbsp;

## Compiling the server
**Linux:**  
See [server/linux/README.md](server/linux/README.md)

**Windows:**  
See [server/windows/README.md](server/windows/README.md)

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

(Optional) Connect a switch to toggle the display backlight like this:  
`GND` -> Switch  
Switch -> `D2`


&nbsp;

## Running
**Linux:**  
Before running, make sure you are in the group uucp (this group name can deviate between distros), otherwise you won't be able to execute the binary without root permissions:  
`sudo usermod -aG uucp $USER`  
You probably need to relog for this change to be applied. You can work around this until you relog by running `su $USER` in the terminal that you'll use to execute the binary in a few moments.  

Go into the directory containing the compiled binary.

Make sure your binary is executable by running:  
`chmod +x ./arduino-resource-monitor-server-linux`  

Connect the Arduino Nano to your PC and wait for it to start.  
Now execute the binary by running `./arduino-resource-monitor-server-linux`. Your LCD display should start displaying values!  

You can send the process into the background to be able to the close the terminal by pressing <kbd>CTRL</kbd>+<kbd>Z</kbd>, running `bg` and then `disown`.

**Windows:**
Open the exe.  
You can also create a small batch script to open the application in the background, maybe even on autostart.

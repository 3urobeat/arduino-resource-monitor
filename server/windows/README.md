# Server for Windows  
Reads resource usage data and sends it to the arduino.  

This server uses the [LibreHardwareMonitor](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor) library to get resource usage information which is licensed under the [MPL 2.0](https://www.mozilla.org/en-US/MPL/2.0/) license.

&nbsp;

## Compiling
Open the solution file `src/server/windows/windows.sln` in Visual Studio.  
Right click on the solution "windows" in your solution explorer and click on Publish.  
There should be a pre-configured configuration shown to you now. Hit Publish at the top and wait until finished.  
Your `.exe` is now located under `src/server/windows/bin/Release/net6.0/publish/win-x64/windows.exe`. Rename and copy the .exe to anywhere you like.

You can probably also compile this project without the full Visual Studio editor installed, but I don't know exactly how.
(I usually develop on Linux)

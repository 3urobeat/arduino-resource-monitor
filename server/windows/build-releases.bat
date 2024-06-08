:: File: build-releases.bat
:: Project: arduino-resource-monitor
:: Created Date: 2024-06-08 14:49:34
:: Author: 3urobeat
::
:: Last Modified: 2024-06-08 17:11:53
:: Modified By: 3urobeat
::
:: Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
::
:: This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
:: This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
:: You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.


:: Clean-Builds all platforms for release


@echo off
setlocal enabledelayedexpansion

:: Get version
set "file=.\src\Settings.cs"			:: Search in
set "search=public const string version "	:: Search for
set "VERSION="

for /f "tokens=*" %%a in ('type "%file%" ^| findstr /c:"%search%"') do (
    set "line=%%a"
    set "line=!line:%search%=!"		:: Remove search string
    set "line=!line:;=!"		:: Remove semicolon
    set "line=!line:"=!"		:: Remove quotation marks
    set "line=!line: =!"		:: Remove remaining spaces
    set "VERSION!line!"			:: Write result into VERSION
)

echo -^> Building Version: %VERSION%


:: Remove old data, ignore "does not exist" warning
rd /s /q .\bin 2>nul
rd /s /q .\obj 2>nul
rd /s /q .\build 2>nul


:: Create build dir for output
mkdir .\build


:: ------- x86_64 -------
echo -^> x86_64: Processing...


:: Remove old data
rd /s /q .\bin 2>nul
rd /s /q .\obj 2>nul

:: Compile and rename release mode
echo -^> x86_64: Compiling release...

dotnet publish -c Release -r win-x64

copy .\bin\Release\net6.0\win-x64\publish\arduino-resource-monitor-server-windows.exe .\build\arduino-resource-monitor-server-v%VERSION%-x86_64-windows.exe


:: Remove old data
rd /s /q .\bin 2>nul
rd /s /q .\obj 2>nul

:: Copy and rename release + clientLessMode mode
echo -^> x86_64: Compiling release + clientLessMode...

dotnet publish -c Release -r win-x64 -p:DefineConstants=BUILD_RELEASE_CLIENT_LESS

copy .\bin\Release\net6.0\win-x64\publish\arduino-resource-monitor-server-windows.exe .\build\arduino-resource-monitor-server-v%VERSION%-x86_64-windows-clientLessMode.exe


:: Exit
echo -^> x86_64: Done!


endlocal	:: Disable delayed execution

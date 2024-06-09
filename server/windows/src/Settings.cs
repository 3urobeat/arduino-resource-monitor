/*
 * File: Settings.cs
 * Project: arduino-resource-monitor
 * Created Date: 2023-11-12 12:59:59
 * Author: 3urobeat
 *
 * Last Modified: 2024-06-09 15:35:23
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


public static class Settings
{
    // Stores currently imported config
    public struct ConfigValues
    {
        // General
        public string createdWithVersion;

        // Timeouts
        public int arduinoReplyTimeout;          // How long to wait for an answer from the USB port in ms before giving up
        public int connectionRetryTimeout;       // How long to wait between attempts to connect again in ms after all USB ports have failed
        public int connectionRetryAmount;        // How often to retry finding a connection
        public double connectionRetryMultiplier; // retry * connectionRetryTimeout * connectionRetryMultiplier

        // Sensors
        public string cpuHardwareName;
        public string gpuHardwareName;

        public string cpuLoadSensorName;
        public string cpuTempSensorName;
        public string gpuLoadSensorName;
        public string gpuTempSensorName;

        public int checkInterval; // How often new measurements should be made, min 1000ms (1 second)
    }

    public static ConfigValues config = new ConfigValues();


    // Configuration variables
    public const bool autoCreateConfig = true;


    // Debug Settings: Force debug and clientLessMode if flag is set, otherwise use your setting
#if RELEASE
    public const bool printDebug = false;
    public const bool clientLessMode = false;
#elif BUILD_RELEASE_CLIENT_LESS
    public const bool printDebug = false;
    public const bool clientLessMode = true;
#elif BUILD_RELEASE_PRINTDEBUG
    public const bool printDebug = true;
    public const bool clientLessMode = false;
#else
    public const bool printDebug = false;           // Prints (a lot of) additional information during runtime, useful for debugging.
    public const bool clientLessMode = false;       // Disables connecting & sending data to the Arduino Client and only logs results to stdout. Useful during development
#endif


    // Do not modify
    public const string version = "0.8.0";

    public const int baud = 9600;
}

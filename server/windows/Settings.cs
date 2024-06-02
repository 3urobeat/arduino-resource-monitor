/*
 * File: Settings.cs
 * Project: arduino-resource-monitor
 * Created Date: 2023-11-12 12:59:59
 * Author: 3urobeat
 *
 * Last Modified: 2024-06-02 15:16:27
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
        public string lastSeenVersion;

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
        public string ramUsedSensorName;
        public string gpuLoadSensorName;
        public string gpuTempSensorName;

        public int checkInterval; // How often new measurements should be made, min 1000ms (1 second)
    }

    public static ConfigValues config = new ConfigValues();


    // Configuration variables
    public const int arduinoReplyTimeout = 5000;         // How long to wait for an answer from the USB port in ms before giving up
    public const int connectionRetryTimeout = 5000;      // How long to wait between attempts to connect again in ms after all USB ports have failed
    public const int connectionRetryAmount = 10;         // How often to retry finding a connection
    public const double connectionRetryMultiplier = 0.5; // retry * connectionRetryTimeout * connectionRetryMultiplier

    public const int checkInterval = 1000; // How often new measurements should be made, min 1000ms (1 second)
    public const int gpuID = 0;            // Set an index here if you've got multiple GPUs. Check the Task Manager Performance tab for reference


    // Sensor names: These might change depending on your hardware
    public const string cpuLoadSensor = "customCpuLoad"; // Name of the sensor
    public const int cpuLoadSensorIndex = 0;             // Set index of the correct sensor should multiple exist

    public const string cpuTempSensor = "customCpuTemp";
    public const int cpuTempSensorIndex = 0;

    public const string ramUsedSensor = "customRam";
    public const int ramUsedSensorIndex = 0;

    public const string gpuLoadSensor = "customGpuLoad";
    public const int gpuLoadSensorIndex = 0;

    public const string gpuTempSensor = "customGpuTemp";
    public const int gpuTempSensorIndex = 0;


    // Configuration variables
    public const bool autoCreateConfig = true;

    public const bool printDebug = true;
    public const bool clientLessMode = false;


    // Do not modify
    public const string version = "0.7.0";

    public const int baud = 9600;
}

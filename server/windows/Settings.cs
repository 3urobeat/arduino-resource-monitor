﻿/*
 * File: Settings.cs
 * Project: arduino-resource-monitor
 * Created Date: 12.11.2023 12:59:59
 * Author: 3urobeat
 *
 * Last Modified: 12.11.2023 17:10:56
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


public class Settings
{
    public const string version = "0.5.1";

    public const int baud = 9600;
    public const int readTimeout = 2500;

    public const int gpuID = 1;            // Set an index here if you've got multiple GPUs. Check the Task Manager Performance tab for reference


    // Sensor names: These might change depending on your hardware
    public const string cpuLoadSensor = "CPU Total"; // Name of the sensor
    public const int cpuLoadSensorIndex = 0;         // Set index of the correct sensor should multiple exist

    public const string cpuTempSensor = "Core (Tctl/Tdie)";
    public const int cpuTempSensorIndex = 0;

    public const string ramUsedSensor = "Memory Used";
    public const int ramUsedSensorIndex = 0;

    public const string gpuLoadSensor = "D3D 3D";
    public const int gpuLoadSensorIndex = 0;

    public const string gpuTempSensor = "GPU Core";
    public const int gpuTempSensorIndex = 0;
}

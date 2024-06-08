/*
 * File: Measurements.cs
 * Project: arduino-resource-monitor
 * Created Date: 2023-11-12 18:13:01
 * Author: 3urobeat
 *
 * Last Modified: 2024-06-08 12:04:45
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


using static Sensors;


// Stores the current measurements
public class MeasurementsCache
{
    public static string cpuLoad = "";
    public static string cpuTemp = "";
    public static string ramUsage = "";
    public static string swapUsage = "";
    public static string gpuLoad = "";
    public static string gpuTemp = "";
}


public class Measurements
{
    // Reads from all sensors and refreshes MeasurementsCache
    public static void GetMeasurements()
    {
        // Update measurements
        CpuHardware?.Update();
        MemHardware?.Update();
        GpuHardware?.Update();

        MainClass.LogDebug("Updating sensor values...");

        // CPU Utilization in %
        if (CpuLoadSensor != null)
        {
            MeasurementsCache.cpuLoad = ((int)CpuLoadSensor.Value!).ToString();  // Ignore decimals
        }

        // CPU Temperature in °C
        if (CpuTempSensor != null)
        {
            MeasurementsCache.cpuTemp = ((int)CpuTempSensor.Value!).ToString(); // Ignore decimals
        }

        // RAM Usage in GB
        if (RamUsedSensor != null)
        {
            MeasurementsCache.ramUsage = Math.Round((float)RamUsedSensor.Value!, 1).ToString(".0");
        }

        // GPU Utilization in %
        if (GpuLoadSensor != null)
        {
            MeasurementsCache.gpuLoad = ((int)GpuLoadSensor.Value!).ToString();
        }

        // GPU Core Temperature in °C
        if (GpuTempSensor != null) {
            MeasurementsCache.gpuTemp = ((int)GpuTempSensor.Value!).ToString();
        }
    }
}

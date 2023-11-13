﻿/*
 * File: Measurements.cs
 * Project: arduino-resource-monitor
 * Created Date: 12.11.2023 18:13:01
 * Author: 3urobeat
 *
 * Last Modified: 12.11.2023 18:21:54
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


using LibreHardwareMonitor.Hardware;
using LibreHardwareMonitor.Hardware.Cpu;


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


// From lib documentation: https://github.com/LibreHardwareMonitor/LibreHardwareMonitor#developer-information
public class UpdateVisitor : IVisitor
{
    public void VisitComputer(IComputer computer)
    {
        computer.Traverse(this);
    }
    public void VisitHardware(IHardware hardware)
    {
        hardware.Update();
        foreach (IHardware subHardware in hardware.SubHardware) subHardware.Accept(this);
    }
    public void VisitSensor(ISensor sensor) { }
    public void VisitParameter(IParameter parameter) { }
}


public class Measurements
{
    static Computer computer = new Computer
    {
        IsCpuEnabled = true,
        IsGpuEnabled = true,
        IsMemoryEnabled = true,
        IsMotherboardEnabled = false, // We don't need everything below
        IsControllerEnabled = false,
        IsNetworkEnabled = false,
        IsStorageEnabled = false
    };


    // Store sensors
    public float CpuUsageSensor { get; private set; }
    public float CpuTempSensor { get; private set; }
    public float RamUsedSensor { get; private set; }
    public float GpuUsageSensor { get; private set; }
    public float GpuTempSensor { get; private set; }


    // Find all sensors and store them above
    public static void FindSensors()
    {
        computer.Open();
        computer.Accept(new UpdateVisitor());

        int gpuId = 0;


        int cpuLoadSensorIndex = 0;
        int cpuTempSensorIndex = 0;
        int ramUsedSensorIndex = 0;
        int gpuLoadSensorIndex = 0;
        int gpuTempSensorIndex = 0;


        // Iterate through every device and its sensors
        foreach (IHardware hardware in computer.Hardware)
        {
            //Console.WriteLine("Hardware: {0}", hardware.Name); // Enable to log all sensors

            // Loop through all sensors of this hardware
            foreach (ISensor sensor in hardware.Sensors)
            {
                //Console.WriteLine("\tSensor: {0}, value: {1}", sensor.Name, sensor.Value); // Enable to log all sensors

                switch (sensor.Name)
                {
                    // CPU Utilization in %
                    case Settings.cpuLoadSensor:
                        if (cpuLoadSensorIndex == Settings.cpuLoadSensorIndex)
                        {
                            MeasurementsCache.cpuLoad = ((int)sensor.Value).ToString() + "%";  // Ignore decimals

                            Console.WriteLine("CPU Util: " + MeasurementsCache.cpuLoad);
                        }

                        cpuLoadSensorIndex++;
                        break;

                    // CPU Temperature in °C
                    case Settings.cpuTempSensor:
                        if (cpuTempSensorIndex == Settings.cpuTempSensorIndex)
                        {
                            MeasurementsCache.cpuTemp = ((int)sensor.Value).ToString() + "°C"; // Ignore decimals

                            Console.WriteLine("CPU Temp: " + MeasurementsCache.cpuTemp);
                        }

                        cpuTempSensorIndex++;
                        break;

                    // RAM Usage in GB
                    case Settings.ramUsedSensor:
                        if (ramUsedSensorIndex == Settings.ramUsedSensorIndex)
                        {
                            MeasurementsCache.ramUsage = Math.Round((float)sensor.Value, 1).ToString() + "GB";

                            Console.WriteLine("RAM: " + MeasurementsCache.ramUsage);
                        }

                        ramUsedSensorIndex++;
                        break;

                    // I don't know how Swap works on Windows so let's ignore this for now

                    // GPU Utilization in %
                    case Settings.gpuLoadSensor:
                        if (gpuId == Settings.gpuID)
                        {
                            if (gpuLoadSensorIndex == Settings.gpuLoadSensorIndex)
                            {
                                MeasurementsCache.gpuLoad = ((int)sensor.Value).ToString() + "%";

                                Console.WriteLine("GPU Util: " + MeasurementsCache.gpuLoad);
                            }

                            gpuLoadSensorIndex++;
                        }
                        break;

                    // GPU Core Temperature in °C
                    case Settings.gpuTempSensor:
                        if (gpuId == Settings.gpuID)
                        {
                            if (gpuTempSensorIndex == Settings.gpuTempSensorIndex)
                            {
                                MeasurementsCache.gpuTemp = ((int)sensor.Value).ToString() + "°C";

                                Console.WriteLine("GPU Temp: " + MeasurementsCache.gpuTemp);
                            }

                            gpuTempSensorIndex++;
                        }
                        break;
                }
            }

            // Count gpuId to support computers with multiple GPUs (e.g. iGPU & dGPU)
            if (hardware.HardwareType is HardwareType.GpuNvidia or HardwareType.GpuAmd or HardwareType.GpuIntel)
            {
                gpuId++;
            }
        }

        computer.Close();
    }
}
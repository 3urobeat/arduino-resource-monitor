/*
 * File: Sensors.cs
 * Project: arduino-resource-monitor
 * Created Date: 2024-06-01 13:32:06
 * Author: 3urobeat
 *
 * Last Modified: 2024-06-02 15:16:16
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


using LibreHardwareMonitor.Hardware;
using static Settings;


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


public static class Sensors
{
    // Init lib
    public static Computer computer = new Computer
    {
        IsCpuEnabled = true,
        IsGpuEnabled = true,
        IsMemoryEnabled = true,
        IsMotherboardEnabled = false, // We don't need everything below
        IsControllerEnabled = false,
        IsNetworkEnabled = false,
        IsStorageEnabled = false
    };

    public static IVisitor updateVisitor = new UpdateVisitor();

    // Store Hardware for Update()
    public static IHardware? CpuHardware { get; private set; }
    public static IHardware? MemHardware { get; private set; }
    public static IHardware? GpuHardware { get; private set; }

    // Store sensors
    public static ISensor? CpuLoadSensor { get; private set; }
    public static ISensor? CpuTempSensor { get; private set; }
    public static ISensor? RamUsedSensor { get; private set; }
    public static ISensor? GpuLoadSensor { get; private set; }
    public static ISensor? GpuTempSensor { get; private set; }


    // Used for printing warning when multiple CPU/GPUs were auto-discovered
    private static bool cpuLoadAutoDiscovered;
    private static bool cpuTempAutoDiscovered;
    private static bool gpuLoadAutoDiscovered;
    private static bool gpuTempAutoDiscovered;


    // Finds interesting CPU sensors from Cpu Hardware
    private static void ProcessCpuSensors(IHardware hardware)
    {
        foreach (ISensor sensor in hardware.Sensors)
        {
            MainClass.LogDebug($"\tCPU Sensor - name: \"{sensor.Name}\", type: {sensor.SensorType}, value: {sensor.Value}");

            // Load
            if (sensor.SensorType == SensorType.Load)
            {
                // Check for user defined sensor name first and overwrite any previous stored sensor
                if (!string.IsNullOrEmpty(config.cpuLoadSensorName) && config.cpuLoadSensorName == sensor.Name)
                {
                    CpuLoadSensor = sensor;
                    Console.WriteLine($"Found pre-configured CPU Load sensor '{sensor.Name}'!");
                }
                else
                {
                    switch (sensor.Name)
                    {
                        case "CPU Total":
                            if (CpuLoadSensor == null)          // How many indentations do you want? Yes.
                            {
                                CpuLoadSensor = sensor;
                                cpuLoadAutoDiscovered = true;
                                Console.WriteLine($"Found CPU Load sensor '{sensor.Name}'!");
                            }
                            else
                            {
                                if (cpuLoadAutoDiscovered) Console.WriteLine("Warning: Your system has multiple CPU load sensors! If the wrong sensor has been chosen, please configure it manually.");
                            }

                            break;
                    }
                }
            }

            // Temperature
            if (sensor.SensorType == SensorType.Temperature)
            {
                // Check for user defined sensor name first and overwrite any previous stored sensor
                if (!string.IsNullOrEmpty(config.cpuTempSensorName) && config.cpuTempSensorName == sensor.Name)
                {
                    CpuTempSensor = sensor;
                    Console.WriteLine($"Found pre-configured CPU Temperature sensor '{sensor.Name}'!");
                }
                else
                {
                    switch (sensor.Name)
                    {
                        case "Core (Tctl/Tdie)": // AMD Ryzen 7000
                        case "CPU Package":      // Intel 4000
                        case "CPU Cores":        // AMD family 1Xh
                            if (CpuTempSensor == null)
                            {
                                CpuTempSensor = sensor;
                                cpuTempAutoDiscovered = true;
                                Console.WriteLine($"Found CPU Temperature sensor '{sensor.Name}'!");
                            }
                            else
                            {
                                if (cpuTempAutoDiscovered) Console.WriteLine("Warning: Your system has multiple CPU temperature sensors! If the wrong sensor has been chosen, please configure it manually.");
                            }

                            break;
                    }
                }
            }
        }
    }

    // Finds interesting RAM sensors from Memory Hardware
    private static void ProcessMemSensors(IHardware hardware)
    {
        foreach (ISensor sensor in hardware.Sensors)
        {
            MainClass.LogDebug($"\tMemory Sensor - name: \"{sensor.Name}\", type: {sensor.SensorType}, value: {sensor.Value}");

            // RAM
            if (sensor.Name == "Memory Used")
            {
                RamUsedSensor = sensor;
                Console.WriteLine($"Found RAM Used sensor '{sensor.Name}'!");
            }
        }
    }

    // Finds interesting GPU sensors from Gpu Hardware
    private static void ProcessGpuSensors(IHardware hardware)
    {
        foreach (ISensor sensor in hardware.Sensors)
        {
            MainClass.LogDebug($"\tGPU Sensor - name: \"{sensor.Name}\", type: {sensor.SensorType}, value: {sensor.Value}");

            // Load
            if (sensor.SensorType == SensorType.Load)
            {
                // Check for user defined sensor name first and overwrite any previous stored sensor
                if (!string.IsNullOrEmpty(config.gpuLoadSensorName) && config.gpuLoadSensorName == sensor.Name)
                {
                    GpuLoadSensor = sensor;
                    Console.WriteLine($"Found pre-configured GPU Load sensor '{sensor.Name}'!");
                }
                else
                {
                    switch (sensor.Name)
                    {
                        case "GPU Core":    // AMD && Nvidia
                        case "D3D 3D":      // Prioritize if available, readouts make more sense, at least on my RX 7900XT
                            if (GpuLoadSensor == null || (GpuLoadSensor.Name == "GPU Core" && sensor.Name == "D3D 3D"))
                            {
                                GpuLoadSensor = sensor;
                                gpuLoadAutoDiscovered = true;
                                Console.WriteLine($"Found GPU Load sensor '{sensor.Name}'!");
                            }
                            else
                            {
                                if (gpuLoadAutoDiscovered) Console.WriteLine("Warning: Your system has multiple GPU load sensors! If the wrong sensor has been chosen, please configure it manually.");
                            }

                            break;
                    }
                }
            }

            // Temperature
            if (sensor.SensorType == SensorType.Temperature)
            {
                // Check for user defined sensor name first and overwrite any previous stored sensor
                if (!string.IsNullOrEmpty(config.gpuTempSensorName) && config.gpuTempSensorName == sensor.Name)
                {
                    GpuTempSensor = sensor;
                    Console.WriteLine($"Found pre-configured GPU Temperature sensor '{sensor.Name}'!");
                }
                else
                {
                    switch (sensor.Name)
                    {
                        case "GPU Core":    // AMD && Nvidia
                            if (GpuTempSensor == null)
                            {
                                GpuTempSensor = sensor;
                                gpuTempAutoDiscovered = true;
                                Console.WriteLine($"Found GPU Temperature sensor '{sensor.Name}'!");
                            }
                            else
                            {
                                if (gpuTempAutoDiscovered) Console.WriteLine("Warning: Your system has multiple GPU temperature sensors! If the wrong sensor has been chosen, please configure it manually.");
                            }

                            break;
                    }
                }
            }
        }
    }


    // Find all sensors and store them above
    public static void FindSensors()
    {
        computer.Open();
        computer.Accept(updateVisitor);

        // Reset
        CpuHardware = null;
        MemHardware = null;
        GpuHardware = null;

        CpuLoadSensor = null;
        CpuTempSensor = null;
        RamUsedSensor = null;
        GpuLoadSensor = null;
        GpuTempSensor = null;

        // Reset trackers
        cpuLoadAutoDiscovered = false;
        cpuTempAutoDiscovered = false;
        gpuLoadAutoDiscovered = false;
        gpuTempAutoDiscovered = false;


        // Iterate through every device and its sensors
        foreach (IHardware hardware in computer.Hardware)
        {
            switch (hardware.HardwareType)
            {
                case HardwareType.Cpu:
                    // Check if specific cpu is defined in config and skip if this does not match
                    if (!string.IsNullOrEmpty(config.cpuHardwareName) && config.cpuHardwareName != hardware.Name)
                    {
                        MainClass.LogDebug($"Found CPU '{hardware.Name}' but skipping because config.cpuHardwareName is defined");
                        continue;
                    }

                    MainClass.LogDebug($"CPU Hardware: {hardware.Name}, type: {hardware.HardwareType}, identifier: {hardware.Identifier}");

                    CpuHardware = hardware;
                    ProcessCpuSensors(hardware);
                    break;

                case HardwareType.Memory:
                    MainClass.LogDebug($"Memory Hardware: {hardware.Name}, type: {hardware.HardwareType}, identifier: {hardware.Identifier}");

                    MemHardware = hardware;
                    ProcessMemSensors(hardware);
                    break;

                case HardwareType.GpuAmd:
                case HardwareType.GpuNvidia:
                case HardwareType.GpuIntel:
                    // Check if specific gpu is defined in config and skip if this does not match
                    if (!string.IsNullOrEmpty(config.gpuHardwareName) && config.gpuHardwareName != hardware.Name)
                    {
                        MainClass.LogDebug($"Found GPU '{hardware.Name}' but skipping because config.gpuHardwareName is defined");
                        continue;
                    }

                    MainClass.LogDebug($"GPU Hardware: {hardware.Name}, type: {hardware.HardwareType}, identifier: {hardware.Identifier}");

                    GpuHardware = hardware;
                    ProcessGpuSensors(hardware);
                    break;
            }
        }


        // Check if any of the sensors could not be found
        if (CpuLoadSensor == null) Console.WriteLine("Warn: I could not automatically find any 'CPU Load' sensor! If you have one, please configure it manually.");
        if (CpuTempSensor == null) Console.WriteLine("Warn: I could not automatically find any 'CPU Temperature' sensor! If you have one, please configure it manually.");
        if (RamUsedSensor == null) Console.WriteLine("Warn: I could not automatically find any 'Memory Used' sensor! If you have one, please configure it manually.");
        if (GpuLoadSensor == null) Console.WriteLine("Warn: I could not automatically find any 'GPU Load' sensor! If you have one, please configure it manually.");
        if (GpuTempSensor == null) Console.WriteLine("Warn: I could not automatically find any 'GPU Temperature' sensor! If you have one, please configure it manually.");


        // Don't call computer.close() here, otherwise GetMeasurements() won't be able to refresh readouts
    }
}

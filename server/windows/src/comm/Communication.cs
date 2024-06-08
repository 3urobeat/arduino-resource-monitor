/*
 * File: Communication.cs
 * Project: arduino-resource-monitor
 * Created Date: 2023-11-13 22:21:38
 * Author: 3urobeat
 *
 * Last Modified: 2024-06-08 12:04:50
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


using static MainClass;


public class Communication
{
    // Specifies the IDs which data messages are prefixed with to indicate their type
    enum MeasurementTypes
    {
        pingID = 0, // Empty data message only used by server for preventing connection loss screen
        cpuLoadID = 1,
        cpuTempID = 2,
        ramUsageID = 3,
        //swapUsageID = 4, // Unsupported by this server
        gpuLoadID = 5,
        gpuTempID = 6
    };


    // Store what we last sent to the Arduino to avoid unnecessary refreshes
    public class ArduinoCache
    {
        public static string cpuLoad = "";
        public static string cpuTemp = "";
        public static string ramUsage = "";
        //public static string swapUsage = ""; // Unsupported by this server
        public static string gpuLoad = "";
        public static string gpuTemp = "";
    }


    private static long lastWriteTime = 0;


    // Sends the actual serial message
    private static void SendSerial(string str, MeasurementTypes id)
    {
        try
        {
            if (serialConnection == null) return;

            serialConnection.Write($"~{(int)id}-{str}#");

            LogDebug("Sending: " + $"~{(int)id}-{str}#");

            lastWriteTime = DateTimeOffset.Now.ToUnixTimeMilliseconds();
        }
        catch (Exception e)
        {
            Console.WriteLine("\x1b[91mError:\x1b[0m Failed to send data to device: " + e.Message);
            Reconnect();
        }
    }


    // Sends current measurements to the Arduino
    public static void SendMeasurements()
    {
        // Send what changed
        if (ArduinoCache.cpuLoad != MeasurementsCache.cpuLoad)
        {
            SendSerial(MeasurementsCache.cpuLoad, MeasurementTypes.cpuLoadID);
            ArduinoCache.cpuLoad = MeasurementsCache.cpuLoad;
        }

        if (ArduinoCache.cpuTemp != MeasurementsCache.cpuTemp)
        {
            SendSerial(MeasurementsCache.cpuTemp, MeasurementTypes.cpuTempID);
            ArduinoCache.cpuTemp = MeasurementsCache.cpuTemp;
        }

        if (ArduinoCache.ramUsage != MeasurementsCache.ramUsage)
        {
            SendSerial(MeasurementsCache.ramUsage, MeasurementTypes.ramUsageID);
            ArduinoCache.ramUsage = MeasurementsCache.ramUsage;
        }

        if (ArduinoCache.gpuLoad != MeasurementsCache.gpuLoad)
        {
            SendSerial(MeasurementsCache.gpuLoad, MeasurementTypes.gpuLoadID);
            ArduinoCache.gpuLoad = MeasurementsCache.gpuLoad;
        }

        if (ArduinoCache.gpuTemp != MeasurementsCache.gpuTemp)
        {
            SendSerial(MeasurementsCache.gpuTemp, MeasurementTypes.gpuTempID);
            ArduinoCache.gpuTemp = MeasurementsCache.gpuTemp;
        }


        // Check if we need to send an alive ping to keep the Arduino from switching to Connection Lost screen
        if (DateTimeOffset.Now.ToUnixTimeMilliseconds() - lastWriteTime > 5000)
        {
            LogDebug("Sending alive ping!");

            SendSerial("", MeasurementTypes.pingID);
        }
    }


    // Logs the current measurements to stdout instead of sending them to the Client
    public static void LogMeasurements()
    {
        Console.WriteLine($"CPU: {MeasurementsCache.cpuLoad}% {MeasurementsCache.cpuTemp}�C");
        Console.WriteLine($"RAM: {MeasurementsCache.ramUsage}GB {MeasurementsCache.swapUsage}GB");
        Console.WriteLine($"GPU: {MeasurementsCache.gpuLoad}% {MeasurementsCache.gpuTemp}�C");
    }


    // Resets the ArduinoCache storage. This causes SendMeasurements() to resend every measurement.
    public static void ResetCache()
    {
        LogDebug("Resetting ArduinoCache...");

        ArduinoCache.cpuLoad = "";
        ArduinoCache.cpuTemp = "";
        ArduinoCache.ramUsage = "";
        ArduinoCache.gpuLoad = "";
        ArduinoCache.gpuTemp = "";
    }

}

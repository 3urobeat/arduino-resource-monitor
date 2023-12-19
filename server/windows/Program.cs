/*
 * File: Program.cs
 * Project: arduino-resource-monitor
 * Created Date: 12.11.2023 11:34:19
 * Author: 3urobeat
 *
 * Last Modified: 18.11.2023 15:14:57
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


using System.Globalization;
using System.IO.Ports;


public class MainClass
{
    public static SerialPort? serialConnection;


    // Entry point
    public static void Main(string[] args)
    {
        // Force CultureInfo to en-US to guarantee the same number formatting on different devices
        Thread.CurrentThread.CurrentCulture = CultureInfo.GetCultureInfo("en-US");


        // Set title and print welcome messages
        Console.Title = $"arduino-resource-monitor Server for Windows v{Settings.version} by 3urobeat";

        Console.WriteLine("arduino-resource-monitor by 3urobeat");
        Console.WriteLine($"Server for Windows v{Settings.version} starting...\n");


        // Validate settings
        if (Settings.checkInterval < 1000)
        {
            Console.WriteLine("Error: Setting checkInterval is too low! Please set it to at least 1000!");
            System.Threading.Thread.Sleep(5000);
            System.Environment.Exit(1);
            return;
        }


        // Find all sensors
        Console.WriteLine("Attempting to find configured sensors...");

        Measurements.FindSensors();


        // Begin
        Connect();

        // So uhhh, this is the most clever thing you'll ever see:
        // This while loop keeps the process alive. Calling async functions without await would cause the process to exit even though the async function is still doing something.
        // Since I don't want to complicate the whole process flow with awaits (as Connect() is also called from try catch in Communication.cs), this should be okay.
        // (I am aware of how async await works and how to use it, this is a "hack" which I'm 100% ok with in this project)
        while (true) { }
    }


    // Attempts to find & connect to device and starts to measure & send data
    public static async Task Connect()
    {
        // Find port our arduino is connected to
        Console.WriteLine("Searching for Arduino...");

        serialConnection = await Connection.ConnectToArduino();

        if (serialConnection == null)
        {
            Console.WriteLine("Couldn't connect! Exiting in 5 seconds...");
            System.Threading.Thread.Sleep(5000);
            System.Environment.Exit(1);
            return;
        }

        Console.WriteLine($"Successfully connected to Arduino on port '{serialConnection.PortName}'!");
        Console.WriteLine("\nStarting to send data...");

        // Get new measurements every checkInterval ms and send them to the Arduino as long as a connection exists
        while (serialConnection != null)
        {
            // Get new measurements
            Measurements.GetMeasurements();

            // Send measurements
            Communication.SendMeasurements();

            // Delay next iteration for checkInterval ms
            await Task.Delay(Settings.checkInterval);
        }
    }


    // Closes connection to the device and attempts to reconnect
    public static void Reconnect()
    {
        Console.WriteLine("\nAttempting to reconnect in 5 seconds...");

        // Close serial port if still open
        if (serialConnection != null && serialConnection.IsOpen)
        {
            serialConnection.Close();
        }

        serialConnection = null;

        System.Threading.Thread.Sleep(5000);

        Connect();
    }


    // Logs debug messages if enabled in Settings
    public static void LogDebug(string str)
    {
        if (Settings.printDebug)
        {
            Console.WriteLine(str);
        }
    }
}

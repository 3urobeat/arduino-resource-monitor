﻿/*
 * File: Program.cs
 * Project: arduino-resource-monitor
 * Created Date: 2023-11-12 11:34:19
 * Author: 3urobeat
 *
 * Last Modified: 2024-06-01 21:07:46
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2024 3urobeat <https://github.com/3urobeat>
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

    private static int connectionRetry = 0;


    // Entry point
    public static void Main(string[] args)
    {
        // Force CultureInfo to en-US to guarantee the same number formatting on different devices
        Thread.CurrentThread.CurrentCulture = CultureInfo.GetCultureInfo("en-US");


        // Set title and print welcome messages
        Console.Title = $"arduino-resource-monitor Server for Windows v{Settings.version} by 3urobeat";

        Console.WriteLine("arduino-resource-monitor by 3urobeat");
        Console.WriteLine($"Server for Windows v{Settings.version} starting...\n");


        // Import config and validate settings
        Config.ImportConfig();
        Console.WriteLine("");

        if (Settings.checkInterval < 1000)
        {
            Console.WriteLine("Error: Setting checkInterval is too low! Please set it to at least 1000!");
            System.Threading.Thread.Sleep(5000);
            System.Environment.Exit(1);
            return;
        }


        // Attempt to find sensors
        Console.WriteLine("Attempting to find configured sensors...");

        Sensors.FindSensors();
        Console.WriteLine("");


        // Begin
        Connect().Wait();
    }


    public static void DataLoop()
    {
        Console.WriteLine("\nStarting to send data...");

        if (!Settings.clientLessMode)
        {
            while (serialConnection != null) // Get new measurements every checkInterval ms and send them to the Arduino as long as a connection exists
            {
                // Get new measurements
                Measurements.GetMeasurements();

                // Send measurements
                Communication.SendMeasurements();

                // Delay next iteration for checkInterval ms
                System.Threading.Thread.Sleep(Settings.checkInterval);
            }
        }
        else
        {
            while (true) // Run forever until process is manually terminated
            {
                // Get new measurements
                Measurements.GetMeasurements();

                // Log measurements
                Communication.LogMeasurements();

                // Delay next iteration for checkInterval ms
                System.Threading.Thread.Sleep(Settings.checkInterval);
            }
        }
    }


    // Attempts to find & connect to device and starts to measure & send data
    public static async Task Connect()
    {
        connectionRetry++;

        // Find port our arduino is connected to
        if (!Settings.clientLessMode)
        {
            Console.WriteLine("Searching for Arduino...");

            serialConnection = await Connection.ConnectToArduino();

            // Retry establishing connection if ConnectToArduino() failed
            if (serialConnection == null)
            {
                if (connectionRetry > Settings.connectionRetryAmount)
                {
                    Console.WriteLine($"Couldn't connect after {connectionRetry} attempts! Exiting in 5 seconds...");
                    System.Threading.Thread.Sleep(5000);
                    System.Environment.Exit(1);
                }

                int delay = (int)(Settings.connectionRetryMultiplier * Settings.connectionRetryTimeout) * (connectionRetry + 1);

                Console.WriteLine($"Couldn't connect! Attempting again in {delay}ms (attempt {connectionRetry + 1}/{Settings.connectionRetryAmount})...");

                System.Threading.Thread.Sleep(delay);
                Connect().Wait();
            }
            else
            {
                Console.WriteLine($"Successfully connected to Arduino on port '{serialConnection.PortName}'!");

                // Start getting and sending sensor data
                Communication.ResetCache();

                DataLoop();
            }
        }
        else
        {
            Console.WriteLine("Skipped searching for Arduino because clientLessMode is enabled!");

            // Start getting and sending sensor data
            Communication.ResetCache();

            DataLoop();
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

        // Reset connection tries
        connectionRetry = 0;

        Connect().Wait();
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

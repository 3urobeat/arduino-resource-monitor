/*
 * File: Program.cs
 * Project: arduino-resource-monitor
 * Created Date: 2023-11-12 11:34:19
 * Author: 3urobeat
 *
 * Last Modified: 2024-06-08 12:04:36
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
using System.Runtime.InteropServices;
using static Settings;


public class MainClass
{
    // Stuff needed to enable color support because Windows is delusional - https://stackoverflow.com/questions/44574274/setconsolemode-fails-with-zero-lasterror-0
    const int STD_OUTPUT_HANDLE = -11;
    const uint ENABLE_VIRTUAL_TERMINAL_PROCESSING = 4;

    [DllImport("kernel32.dll", SetLastError = true)]
    static extern IntPtr GetStdHandle(int nStdHandle);

    [DllImport("kernel32.dll")]
    static extern bool GetConsoleMode(IntPtr hConsoleHandle, out uint lpMode);

    [DllImport("kernel32.dll")]
    static extern bool SetConsoleMode(IntPtr hConsoleHandle, uint dwMode);


    public static SerialPort? serialConnection;

    private static int connectionRetry = 0;


    // Entry point
    public static void Main(string[] args)
    {
        // Force CultureInfo to en-US to guarantee the same number formatting on different devices
        Thread.CurrentThread.CurrentCulture = CultureInfo.GetCultureInfo("en-US");

        // Manually enable color support - Windows is fucking stupid - https://stackoverflow.com/questions/44574274/setconsolemode-fails-with-zero-lasterror-0
        IntPtr handle = GetStdHandle(STD_OUTPUT_HANDLE);
        uint mode;
        GetConsoleMode(handle, out mode);
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(handle, mode);


        // Set title and print welcome messages
        Console.Title = $"arduino-resource-monitor Server for Windows v{Settings.version} by 3urobeat";

        Console.WriteLine("\x1b[96marduino-resource-monitor\x1b[0m by 3urobeat");
        Console.WriteLine($"Server for Windows v{Settings.version} starting...\n");


        // Import config and validate settings
        Config.ImportConfig();
        Console.WriteLine("");

        if (config.checkInterval < 1000)
        {
            Console.WriteLine("\x1b[91mError:\x1b[0m Setting checkInterval is too low! Please set it to at least 1000!");
            System.Threading.Thread.Sleep(5000);
            System.Environment.Exit(1);
            return;
        }


        // Attempt to find sensors
        Console.WriteLine("Attempting to discover hardware sensors...");

        Sensors.FindSensors();
        Console.WriteLine("");


        // Begin
        Connect();
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
                System.Threading.Thread.Sleep(config.checkInterval);
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
                System.Threading.Thread.Sleep(config.checkInterval);
            }
        }
    }


    // Attempts to find & connect to device and starts to measure & send data
    public static void Connect()
    {
        connectionRetry++;

        // Find port our arduino is connected to
        if (!Settings.clientLessMode)
        {
            Console.WriteLine("Searching for Arduino...");

            serialConnection = Connection.ConnectToArduino();

            // Retry establishing connection if ConnectToArduino() failed
            if (serialConnection == null)
            {
                if (connectionRetry > config.connectionRetryAmount)
                {
                    Console.WriteLine($"\u001b[91mError:\u001b[0m Couldn't connect after {connectionRetry} attempts! Exiting in 5 seconds...");
                    System.Threading.Thread.Sleep(5000);
                    System.Environment.Exit(1);
                }

                int delay = (int)(config.connectionRetryMultiplier * config.connectionRetryTimeout) * (connectionRetry + 1);

                Console.WriteLine($"\u001b[91mError:\u001b[0m Couldn't connect! Attempting again in {delay}ms (attempt {connectionRetry + 1}/{config.connectionRetryAmount})...");

                System.Threading.Thread.Sleep(delay);
                Connect();
            }
            else
            {
                Console.WriteLine($"\u001b[92mSuccessfully connected to Arduino on port '{serialConnection.PortName}'!\u001b[0m");

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

        Connect();
    }


    // Logs debug messages if enabled in Settings
    public static void LogDebug(string str)
    {
        if (Settings.printDebug)
        {
            Console.WriteLine("[\u001b[96mDEBUG\u001b[0m] " + str);
        }
    }
}

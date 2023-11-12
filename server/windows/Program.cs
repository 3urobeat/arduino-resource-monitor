/*
 * File: Program.cs
 * Project: arduino-resource-monitor
 * Created Date: 12.11.2023 11:34:19
 * Author: 3urobeat
 *
 * Last Modified: 12.11.2023 17:08:02
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


using System.IO.Ports;


public class MainClass
{
    // Entry point
    public static async Task Main(string[] args)
    {
        Console.Title = $"arduino-resource-monitor Server for Windows v{Settings.version} by 3urobeat";

        Console.WriteLine("arduino-resource-monitor by 3urobeat");
        Console.WriteLine($"Server for Windows v{Settings.version} starting...\n");
        Console.WriteLine("Searching for Arduino...");


        // Find port our arduino is connected to
        SerialPort? serialConnection = await FindArduino.GetArduinoPort();

        if (serialConnection == null)
        {
            Console.WriteLine("Couldn't connect! Exiting in 5 seconds...");
            System.Threading.Thread.Sleep(5000);
            System.Environment.Exit(1);
            return;
        }

        Console.WriteLine($"Successfully connected to Arduino on port '{serialConnection.PortName}'!");
    }
}

﻿/*
 * File: FindArduino.cs
 * Project: arduino-resource-monitor
 * Created Date: 12.11.2023 11:58:51
 * Author: 3urobeat
 *
 * Last Modified: 12.11.2023 17:09:25
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


using Microsoft.Win32;
using System.IO.Ports;

public class FindArduino
{
    static SerialPort? _serialPort;


    private readonly static string[] _arduinoKeys = {
        @"HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\usbser\Enum",
        @"HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\CH341SER_A64\Enum"
    };

    // See https://arduino.stackexchange.com/questions/30808/how-to-detect-arduino-serial-port-programmatically-on-different-platforms/80887
    private static IEnumerable<(string name, string port)> EnumerateArduinos()
    {
        foreach (string arduinoKey in _arduinoKeys)
        {
            var countObject = Registry.GetValue(arduinoKey, "Count", null);

            if (countObject is not int count) continue;

            for (int i = 0; i < count; i++)
            {
                var enumObject = Registry.GetValue(arduinoKey, i.ToString(), null);

                if (enumObject is not string enumKey) continue;

                var friendlyName = Registry.GetValue($@"HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\{enumKey}", "FriendlyName", null);
                var portName     = Registry.GetValue($@"HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\{enumKey}\Device Parameters", "PortName", null);

                if (portName is string port && friendlyName is string name)
                {
                    yield return (name, port);
                }
            }
        }
    }


    // Attempts to find and connect to the client. Returns the connection on success or null on failure.
    public static async Task<SerialPort?> GetArduinoPort()
    {
        int i = 0;
        var ports = EnumerateArduinos().ToList();

        Console.WriteLine($"Found {ports.Count} eligible device(s)...");

        // Try to connect to each arduino
        foreach (var (name, port) in ports)
        {
            i++;

            char[] buffer = new char[32];

            Console.WriteLine($"Attempting to connect to port '{port}', timeout is set to {Settings.readTimeout}ms...");

            // Attempt to connect to this port
            try
            {
                _serialPort = new SerialPort();

                // Set connection information and attempt to open connection
                _serialPort.PortName    = port;
                _serialPort.BaudRate    = Settings.baud;
                _serialPort.ReadTimeout = Settings.readTimeout;

                _serialPort.Open();

                await Task.Delay(100); // Wait a short moment before transmitting after making a connection


                // Attempt to send client our header. A header starts with a +, normal data with a ~
                _serialPort.WriteLine($"+0ResourceMonitorWindowsServer-v{Settings.version}#");

                // Attempt to read response. Stop if connection got closed or buffer is exceeded. ReadTimeout has been set above
                int offset = 0;

                while (_serialPort.IsOpen && offset < 32)
                {
                    buffer[offset] = (char) _serialPort.ReadByte(); // Cast byte to char
                    offset++;
                }


                // Check the response
                string responseStr = new string(buffer); // Construct new string using the buffer char array

                if (responseStr.Length == 0 || !responseStr.StartsWith("+0ResourceMonitorClient")) // Check response
                {
                    throw new Exception("Received invalid response from client: " + responseStr);
                }


                // Break loop if catch didn't trigger, aka a connection was made successfully
                Console.WriteLine("Received valid response from client: " + responseStr);
                break;
            }
            catch (Exception e)
            {
                Console.WriteLine($"Failed to connect to device {i} at '{port}': {e.Message}");

                // Close serial port if still open
                if (_serialPort != null && _serialPort.IsOpen)
                {
                    _serialPort.Close();
                }

                _serialPort = null;
            }
        }

        return _serialPort;
    }
}

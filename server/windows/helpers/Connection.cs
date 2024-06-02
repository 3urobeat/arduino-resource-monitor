/*
 * File: Connection.cs
 * Project: arduino-resource-monitor
 * Created Date: 2023-11-12 11:58:51
 * Author: 3urobeat
 *
 * Last Modified: 2024-06-02 15:33:47
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


using Microsoft.Win32;
using System.IO.Ports;
using static Settings;


public class Connection
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
    public static SerialPort? ConnectToArduino()
    {
        int i = 0;
        var ports = EnumerateArduinos().ToList();

        if (ports.Count == 0)
        {
            Console.WriteLine($"Found no devices! Exiting...");
            System.Threading.Thread.Sleep(5000);
            System.Environment.Exit(1);
        }

        Console.WriteLine($"Found {ports.Count} eligible device(s)...");


        // Try to connect to each arduino
        foreach (var (name, port) in ports)
        {
            i++;

            char[] buffer = new char[32];

            MainClass.LogDebug($"Attempting to connect to port '{port}', timeout is set to {config.arduinoReplyTimeout}ms...");

            // Attempt to connect to this port
            try
            {
                _serialPort = new SerialPort();

                // Set connection information and attempt to open connection
                _serialPort.PortName    = port;
                _serialPort.BaudRate    = Settings.baud;
                _serialPort.ReadTimeout = config.arduinoReplyTimeout;

                _serialPort.Open();

                System.Threading.Thread.Sleep(2500); // Wait a short moment before transmitting in case the Arduino resets upon connection


                // Attempt to send client our header. A header starts with a +, normal data with a ~
                _serialPort.WriteLine($"+ResourceMonitorWindowsServer-v{Settings.version}#");

                // Attempt to read response. Stop if connection got closed or buffer is exceeded. arduinoReplyTimeout has been set above
                int offset = 0;

                while (_serialPort.IsOpen && offset < 32)
                {
                    buffer[offset] = (char) _serialPort.ReadByte(); // Cast byte to char
                    offset++;
                }


                // Check the response and compare version
                string responseStr = new string(buffer); // Construct new string using the buffer char array

                if (responseStr.Length == 0 || !responseStr.StartsWith("+ResourceMonitorClient")) // Check response
                {
                    throw new Exception("Received invalid response from client: " + responseStr);
                }

                string clientVersion = responseStr.Split("-")[1].Replace("#", "").Trim();

                if (clientVersion != ("v" + Settings.version))
                {
                    throw new Exception($"Version mismatch! Client runs on {clientVersion} but we are on v{Settings.version}!");
                }


                // Break loop if catch didn't trigger, aka a connection was made successfully
                MainClass.LogDebug("Received valid response from client: " + responseStr);
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

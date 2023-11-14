/*
 * File: Communication.cs
 * Project: arduino-resource-monitor
 * Created Date: 13.11.2023 22:21:38
 * Author: 3urobeat
 *
 * Last Modified: 14.11.2023 21:44:46
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


using System.IO.Ports;


public class Communication
{
    // Stores what is currently displayed on the Arduino's screen. This allows us to check and avoid sending unnecessary data
    private static string[] lcdCache = new string[4];

    private static long lastWriteTime = 0;


    // Positions a str with the last char to a fixed column by adding spaces and returns it
    private static string StrFixedRight(string strContent, string strToAdd, int column)
    {
        int spacesToAdd = (column - 1) - ((strContent.Length - 1) + (strToAdd.Length - 1));

        for (int i = 0; i < spacesToAdd + 1; i++)
        {
            strContent += ' ';
        }

        return strContent + strToAdd;
    }


    // Sends current measurements to the Arduino
    public static async void SendMeasurements(SerialPort serialConnection)
    {
        string tempStr;


        // Send each row separately
        for (int i = 0; i < 4; i++)
        {
            // Construct line to send. It consists of a data control char ~, the line number, the data and the end control char #
            tempStr = $"~{i}";

            switch (i)
            {
                case 0:
                    tempStr += "Resource Monitor   ";
                    break;
                case 1:
                    tempStr += "CPU:";
                    tempStr = StrFixedRight(tempStr, MeasurementsCache.cpuLoad, 10);
                    tempStr = StrFixedRight(tempStr, MeasurementsCache.cpuTemp, 18);
                    break;
                case 2:
                    tempStr += "RAM:";
                    tempStr = StrFixedRight(tempStr, MeasurementsCache.ramUsage, 11);
                    break;
                case 3:
                    tempStr += "GPU:";
                    tempStr = StrFixedRight(tempStr, MeasurementsCache.gpuLoad, 10);
                    tempStr = StrFixedRight(tempStr, MeasurementsCache.gpuTemp, 18);
                    break;
            }

            tempStr = StrFixedRight(tempStr, " ", 20); // Fill blank space to make sure
            tempStr += "#";


            // Send string if it changed and update time & content tracking variables
            if (tempStr != lcdCache[i])
            {
                MainClass.LogDebug("Sending: " + tempStr);

                lastWriteTime = DateTimeOffset.Now.ToUnixTimeMilliseconds();
                lcdCache[i]   = tempStr;

                serialConnection.Write(tempStr);
            }


            // Wait a moment to let the Arduino relax
            await Task.Delay(250);
        }


        // Check if we need to send an alive ping to keep the Arduino from switching to Connection Lost screen
        if (DateTimeOffset.Now.ToUnixTimeMilliseconds() - lastWriteTime > 5000)
        {
            MainClass.LogDebug("Sending alive ping!");

            lastWriteTime = DateTimeOffset.Now.ToUnixTimeMilliseconds();

            serialConnection.Write("~0Resource Monitor   #");
        }
    }

}

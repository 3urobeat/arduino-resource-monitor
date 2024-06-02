/*
 * File: Config.cs
 * Project: arduino-resource-monitor
 * Created Date: 2024-06-01 17:00:48
 * Author: 3urobeat
 *
 * Last Modified: 2024-06-02 15:16:11
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


using Microsoft.Extensions.Configuration;
using static Settings;


public class Config
{
    private static readonly string defaultConfig =
$@"[general]
lastSeenVersion = ""{Settings.version}""

[timeouts]
arduinoReplyTimeout = 5000
connectionRetryTimeout = 5000
connectionRetryAmount = 10
connectionRetryMultiplier = 0.5

[sensors]
cpuHardwareName = """"
gpuHardwareName = """"
cpuLoadSensorName = """"
cpuTempSensorName = """"
ramUsedSensorName = """"
gpuLoadSensorName = """"
gpuTempSensorName = """"
checkInterval = 1000";

    private static readonly string configPath = Environment.GetFolderPath(System.Environment.SpecialFolder.LocalApplicationData) + "\\arduino-resource-monitor\\config.ini";


    // Parses config content and fills config struct
    private static void ParseConfig(IConfigurationBuilder builder)
    {
        try
        {
            IConfigurationRoot conf = builder.Build();

            string? tempStr;
            int tempInt;
            float tempFloat;
            bool success;


            // Get 'general' values
            tempStr = conf["general:lastSeenVersion"];
            if (!string.IsNullOrEmpty(tempStr)) config.lastSeenVersion = tempStr;


            // Get 'timeouts' values
            success = int.TryParse(conf["timeouts:arduinoReplyTimeout"], out tempInt);
            if (success) config.arduinoReplyTimeout = tempInt;

            success = int.TryParse(conf["timeouts:connectionRetryTimeout"], out tempInt);
            if (success) config.connectionRetryTimeout = tempInt;

            success = int.TryParse(conf["timeouts:connectionRetryAmount"], out tempInt);
            if (success) config.connectionRetryAmount = tempInt;

            success = float.TryParse(conf["timeouts:connectionRetryMultiplier"], out tempFloat);
            if (success) config.connectionRetryMultiplier = tempFloat;


            // Get 'sensors' values
            tempStr = conf["sensors:cpuHardwareName"];
            if (!string.IsNullOrEmpty(tempStr)) config.cpuHardwareName = tempStr;

            tempStr = conf["sensors:gpuHardwareName"];
            if (!string.IsNullOrEmpty(tempStr)) config.gpuHardwareName = tempStr;

            tempStr = conf["sensors:cpuLoadSensorName"];
            if (!string.IsNullOrEmpty(tempStr)) config.cpuLoadSensorName = tempStr;

            tempStr = conf["sensors:cpuTempSensorName"];
            if (!string.IsNullOrEmpty(tempStr)) config.cpuTempSensorName = tempStr;

            tempStr = conf["sensors:ramUsedSensorName"];
            if (!string.IsNullOrEmpty(tempStr)) config.ramUsedSensorName = tempStr;

            tempStr = conf["sensors:gpuLoadSensorName"];
            if (!string.IsNullOrEmpty(tempStr)) config.gpuLoadSensorName = tempStr;

            tempStr = conf["sensors:gpuTempSensorName"];
            if (!string.IsNullOrEmpty(tempStr)) config.gpuTempSensorName = tempStr;

            success = int.TryParse(conf["sensors:checkInterval"], out tempInt);
            if (success) config.checkInterval = tempInt;
        }
        catch (Exception ex)
        {
            Console.WriteLine("Failed to parse config! Error: " + ex.ToString());
            return;
        }
    }


    // Loads default settings into config struct and overwrites them with user settings
    public static void ImportConfig()
    {
        // Load default config into builder
        MainClass.LogDebug($"ImportConfig(): Loading default config...");

        MemoryStream configStream = new MemoryStream();         // Create stream from defaultConfig string so we can dump it into builder
        StreamWriter writer = new StreamWriter(configStream);
        writer.Write(defaultConfig);
        writer.Flush();
        configStream.Position = 0;

        IConfigurationBuilder defaultConfigBuilder = new ConfigurationBuilder().AddIniStream(configStream);

        ParseConfig(defaultConfigBuilder);
    }
}

/*
 * File: Config.cs
 * Project: arduino-resource-monitor
 * Created Date: 2024-06-01 17:00:48
 * Author: 3urobeat
 *
 * Last Modified: 2024-06-07 18:59:53
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
createdWithVersion = ""{Settings.version}""

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
            tempStr = conf["general:createdWithVersion"];
            if (!string.IsNullOrEmpty(tempStr)) config.createdWithVersion = tempStr;


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

            tempStr = conf["sensors:gpuLoadSensorName"];
            if (!string.IsNullOrEmpty(tempStr)) config.gpuLoadSensorName = tempStr;

            tempStr = conf["sensors:gpuTempSensorName"];
            if (!string.IsNullOrEmpty(tempStr)) config.gpuTempSensorName = tempStr;

            success = int.TryParse(conf["sensors:checkInterval"], out tempInt);
            if (success) config.checkInterval = tempInt;
        }
        catch (Exception ex)
        {
            Console.WriteLine("\x1b[91mError:\x1b[0m Failed to parse config! Error: " + ex.ToString());
            return;
        }
    }


    // Loads default settings into config struct and overwrites them with user settings
    public static void ImportConfig()
    {
        // Load default config into builder
        Console.WriteLine("Loading default configuration...");

        MemoryStream configStream = new MemoryStream();         // Create stream from defaultConfig string so we can dump it into builder
        StreamWriter writer = new StreamWriter(configStream);
        writer.Write(defaultConfig);
        writer.Flush();
        configStream.Position = 0;

        IConfigurationBuilder defaultConfigBuilder = new ConfigurationBuilder().AddIniStream(configStream);

        ParseConfig(defaultConfigBuilder);


        // Check if we need to create the config
        if (!File.Exists(configPath))
        {
            if (Settings.autoCreateConfig)
            {
                MainClass.LogDebug($"ImportConfig(): Config file does not exist yet, creating '{configPath}'...");

                // Create underlying directory if necessary
                System.IO.FileInfo file = new System.IO.FileInfo(configPath);
                file.Directory.Create();

                // Write default config to file
                System.IO.File.WriteAllText(configPath, defaultConfig);
            }
            else
            {
                Console.WriteLine($"No config found at '{configPath}', continuing with default settings...");
            }

            return; // No matter if default config has been written or no config exists, we don't need to parse again as there logically cannot be any modification to the default config
        }

        // Load config file
        Console.WriteLine($"Loading user configuration from '{configPath}'...");

        IConfigurationBuilder userConfigBuilder = new ConfigurationBuilder().AddIniFile(configPath, false, true);

        ParseConfig(userConfigBuilder);

        // Log info if pre-configured sensor names have been found
        if (!string.IsNullOrEmpty(config.cpuHardwareName)) Console.WriteLine($"Found cpu restriction in config. Only accepting sensors from '{config.cpuHardwareName}'.");
        if (!string.IsNullOrEmpty(config.gpuHardwareName)) Console.WriteLine($"Found gpu restriction in config. Only accepting sensors from '{config.gpuHardwareName}'.");
        if (!string.IsNullOrEmpty(config.cpuLoadSensorName)) Console.WriteLine($"Found pre-configured cpuLoad sensor name in config: '{config.cpuLoadSensorName}'!");
        if (!string.IsNullOrEmpty(config.cpuTempSensorName)) Console.WriteLine($"Found pre-configured cpuTemp sensor name in config: '{config.cpuTempSensorName}'!");
        if (!string.IsNullOrEmpty(config.gpuLoadSensorName)) Console.WriteLine($"Found pre-configured gpuLoad sensor name in config: '{config.gpuLoadSensorName}'!");
        if (!string.IsNullOrEmpty(config.gpuTempSensorName)) Console.WriteLine($"Found pre-configured gpuTemp sensor name in config: '{config.gpuTempSensorName}'!");
    }
}

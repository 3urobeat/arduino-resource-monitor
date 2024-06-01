﻿/*
 * File: Config.cs
 * Project: arduino-resource-monitor
 * Created Date: 2024-06-01 17:00:48
 * Author: 3urobeat
 *
 * Last Modified: 2024-06-01 17:35:08
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


using static Settings;

public class Config
{
    public static void ImportConfig()
    {
        config.cpuLoadSensorName = "";
        config.cpuTempSensorName = "";
        config.ramUsedSensorName = "";
        config.gpuLoadSensorName = "";
        config.gpuTempSensorName = "";
    }
}

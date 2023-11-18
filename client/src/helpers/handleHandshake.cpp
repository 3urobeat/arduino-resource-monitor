/*
 * File: handshake.cpp
 * Project: arduino-resource-monitor
 * Created Date: 17.11.2023 17:18:57
 * Author: 3urobeat
 *
 * Last Modified: 18.11.2023 14:32:43
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"


// Processes header received from server, checks version and replies
void handleConnectionHandshake(char *serverHandshake) {

    // Send success header
    Serial.print("+ResourceMonitorClient-");
    Serial.print(version);
    Serial.println("#");

}

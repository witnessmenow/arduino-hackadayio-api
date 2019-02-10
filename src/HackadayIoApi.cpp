/*
   Copyright (c) 2018 Brian Lough. All right reserved.

   HackadayIoApi - An Arduino library to wrap the Tindie API

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "HackadayIoApi.h"

HackadayIoApi::HackadayIoApi(Client &client, char *apiKey)
{
    this->client = &client;
    this->_apiKey = apiKey;
}

bool HackadayIoApi::makeGetRequest(char *command)
{
    client->flush();
    client->setTimeout(HACKADAYIO_TIMEOUT);
    if (!client->connect(HACKADAYIO_HOST, portNumber))
    {
        Serial.println(F("Connection failed"));
        return false;
    }

    // give the esp a breather
    yield();

    // Send HTTP request
    client->print(F("GET "));
    client->print(command);
    client->println(F(" HTTP/1.1"));

    //Headers
    client->print(F("Host: "));
    client->println(HACKADAYIO_HOST);

    client->println(F("Cache-Control: no-cache"));

    if (client->println() == 0)
    {
        Serial.println(F("Failed to send request"));
        return false;
    }

    // Check HTTP status
    char status[32] = {0};
    client->readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status, "HTTP/1.1 200 OK") != 0)
    {
        Serial.print(F("Unexpected response: "));
        Serial.println(status);
        return false;
    }

    // Skip HTTP headers
    char endOfHeaders[] = "\r\n\r\n";
    if (!client->find(endOfHeaders))
    {
        Serial.println(F("Invalid response"));
        return false;
    }

    // Was getting stray characters between the headers and the body
    // This should toss them away
    while (client->available() && client->peek() != '{')
    {
        char c = 0;
        client->readBytes(&c, 1);
        if (_debug)
        {
            Serial.print("Tossing an unexpected character: ");
            Serial.println(c);
        }
    }

    // Let the caller of this method parse the JSon from the client
    return true;
}

ProjectInfo HackadayIoApi::getProjectInfo(int projectId)
{
    char command[75];
    sprintf(command, HACKADAYIO_PROJECTS_END_POINT, projectId, _apiKey);   

    if (_debug)
    {
        Serial.println(command);
    }

    // Get from https://arduinojson.org/v5/assistant/
    const size_t bufferSize = JSON_ARRAY_SIZE(tagArraySize) + JSON_OBJECT_SIZE(19);
    ProjectInfo projectInfo;
    // This flag will get cleared if all goes well
    projectInfo.error = true;
    if (makeGetRequest(command))
    {
        // Allocate JsonBuffer
        DynamicJsonBuffer jsonBuffer(bufferSize);

        // Parse JSON object
        JsonObject &root = jsonBuffer.parseObject(*client);
        if (root.success())
        {
            projectInfo.id = root["id"].as<int>();
            projectInfo.url = (char *) root["url"].as<char *>(); 
            projectInfo.owner_id = root["owner_id"].as<int>();
            projectInfo.name = (char *) root["name"].as<char *>();
            projectInfo.summary = (char *) root["summary"].as<char *>();
            //projectInfo.description = (char *) root["description"].as<char *>();
            projectInfo.image_url = (char *) root["image_url"].as<char *>();
            projectInfo.views = root["views"].as<long>();
            projectInfo.comments = root["comments"].as<int>();
            projectInfo.followers = root["followers"].as<int>();
            projectInfo.skulls = root["skulls"].as<int>();
            //projectInfo.logs = root["logs"].as<int>();
            //projectInfo.details = root["details"].as<int>();
            //projectInfo.instruction = root["instruction"].as<int>();
            //projectInfo.components = root["components"].as<int>();
            //projectInfo.images = root["images"].as<int>();
            projectInfo.created = root["created"].as<long>();
            projectInfo.updated = root["updated"].as<long>();

            projectInfo.error = false;
        }
        else
        {
            Serial.println(F("Parsing failed!"));
        }
    }
    closeClient();
    return projectInfo;
}

void HackadayIoApi::closeClient()
{
    if (client->connected())
    {
        if (_debug)
        {
            Serial.println(F("Closing client"));
        }
        client->stop();
    }
}
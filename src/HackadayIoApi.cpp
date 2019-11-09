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

HackadayIoApi::HackadayIoApi(Client& client, const char* apiKey)
    : client(&client), _apiKey(apiKey)
{
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

    // Get from https://arduinojson.org/v6/assistant/
    const size_t bufferSize = JSON_ARRAY_SIZE(tagArraySize) + JSON_OBJECT_SIZE(19) + addtionalStringSize ;
    ProjectInfo projectInfo;
    // This flag will get cleared if all goes well
    projectInfo.error = true;
    if (makeGetRequest(command))
    {
        // Allocate DynamicJsonDocument
        DynamicJsonDocument doc(bufferSize);

        // Parse JSON object
        DeserializationError error = deserializeJson(doc, *client);
        if (!error)
        {
            projectInfo.id = doc["id"].as<int>();
            projectInfo.url = (char *) doc["url"].as<char *>(); 
            projectInfo.owner_id = doc["owner_id"].as<int>();
            projectInfo.name = (char *) doc["name"].as<char *>();
            projectInfo.summary = (char *) doc["summary"].as<char *>();
            //projectInfo.description = (char *) doc["description"].as<char *>();
            projectInfo.image_url = (char *) doc["image_url"].as<char *>();
            projectInfo.views = doc["views"].as<long>();
            projectInfo.comments = doc["comments"].as<int>();
            projectInfo.followers = doc["followers"].as<int>();
            projectInfo.skulls = doc["skulls"].as<int>();
            //projectInfo.logs = doc["logs"].as<int>();
            //projectInfo.details = doc["details"].as<int>();
            //projectInfo.instruction = doc["instruction"].as<int>();
            //projectInfo.components = doc["components"].as<int>();
            //projectInfo.images = doc["images"].as<int>();
            projectInfo.created = doc["created"].as<time_t>();
            projectInfo.updated = doc["updated"].as<time_t>();

            projectInfo.error = false;
        }
        else
        {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(error.c_str());
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